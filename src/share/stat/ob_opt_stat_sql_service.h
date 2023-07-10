/**
 * Copyright (c) 2021 OceanBase
 * OceanBase CE is licensed under Mulan PubL v2.
 * You can use this software according to the terms and conditions of the Mulan PubL v2.
 * You may obtain a copy of Mulan PubL v2 at:
 *          http://license.coscl.org.cn/MulanPubL-2.0
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
 * EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
 * MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
 * See the Mulan PubL v2 for more details.
 */

#ifndef _OB_OPT_STAT_SQL_SERVICE_H_
#define _OB_OPT_STAT_SQL_SERVICE_H_

#include "share/stat/ob_opt_column_stat.h"
#include "share/stat/ob_opt_table_stat.h"
#include "share/stat/ob_stat_define.h"
#include "lib/mysqlclient/ob_mysql_transaction.h"
namespace oceanbase {
namespace common {
namespace sqlclient
{
class ObMySQLResult;
}
class ObServerConfig;
class ObMySQLProxy;

struct ObOptKeyInfo
{
  ObOptKeyInfo() : table_id_(0), partition_id_(0), column_id_(0) {}
  ObOptKeyInfo(const uint64_t table_id,
               const int64_t partition_id,
               const uint64_t column_id)
    : table_id_(table_id),
      partition_id_(partition_id),
      column_id_(column_id)
  {
  }
  uint64_t hash() const
  {
    return common::murmurhash(this, sizeof(ObOptKeyInfo), 0);
  }
  bool operator==(const ObOptKeyInfo &other) const
  {
    return table_id_ == other.table_id_ &&
           partition_id_ == other.partition_id_ &&
           column_id_ == other.column_id_;
  }
  uint64_t table_id_;
  int64_t partition_id_;
  uint64_t column_id_;
  TO_STRING_KV(K(table_id_),
               K(partition_id_),
               K(column_id_));
};

struct ObOptKeyColumnStat
{
  ObOptKeyColumnStat() : key_(NULL), stat_(NULL) {}
  const ObOptColumnStat::Key *key_;
  ObOptColumnStat *stat_;
  TO_STRING_KV(KPC_(key),
               KPC_(stat));
};
/**
 * SQL Service for fetching/updating table level statistics and column level statistics
 */
class ObOptStatSqlService
{
public:
  ObOptStatSqlService();
  ~ObOptStatSqlService();
  bool is_inited() const { return inited_; }
  int init(ObMySQLProxy *proxy, ObServerConfig *config);

  int fetch_table_stat(const uint64_t tenant_id,
                       const ObOptTableStat::Key &key,
                       ObIArray<ObOptTableStat> &all_part_stats);

  int fill_table_stat(sqlclient::ObMySQLResult &result, ObOptTableStat &stat);

  int fill_column_stat(ObIAllocator &allocator,
                       common::sqlclient::ObMySQLResult &result,
                       hash::ObHashMap<ObOptKeyInfo, int64_t> &key_index_map,
                       ObIArray<ObOptKeyColumnStat> &key_col_stats,
                       ObIArray<ObOptKeyColumnStat> &need_hist_key_col_stats);
  int fetch_column_stat(const uint64_t tenant_id,
                        ObIAllocator &allocator,
                        ObIArray<ObOptKeyColumnStat> &key_col_stats);

  int update_table_stat(const uint64_t tenant_id,
                        const ObOptTableStat *tab_stat,
                        const bool is_index_stat);
  int update_table_stat(const uint64_t tenant_id,
                        ObMySQLTransaction &trans,
                        const common::ObIArray<ObOptTableStat*> &table_stats,
                        const int64_t current_time,
                        const bool is_index_stat,
                        const bool is_history_stat = false);
  int update_column_stat(share::schema::ObSchemaGetterGuard *schema_guard,
                         const uint64_t exec_tenant_id,
                         ObMySQLTransaction &trans,
                         const common::ObIArray<ObOptColumnStat*> &column_stats,
                         const int64_t current_time,
                         bool only_update_col_stat = false,
                         bool is_history_stat = false,
                         const ObObjPrintParams &print_params = ObObjPrintParams());

  int delete_table_stat(const uint64_t exec_tenant_id,
                        const uint64_t table_id,
                        const ObIArray<int64_t> &part_ids,
                        const bool cascade_column,
                        int64_t &affected_rows);

  int delete_column_stat(const uint64_t exec_tenant_id,
                         const uint64_t table_id,
                         const ObIArray<uint64_t> &column_ids,
                         const ObIArray<int64_t> &partition_ids,
                         const bool only_histogram /*=false*/);

  static int get_decompressed_llc_bitmap(ObIAllocator &allocator,
                                         const char *comp_buf,
                                         int64_t comp_size,
                                         char *&bitmap_buf,
                                         int64_t &bitmap_size);

  static int get_compressed_llc_bitmap(ObIAllocator &allocator,
                                       const char *bitmap_buf,
                                       int64_t bitmap_size,
                                       char *&comp_buf,
                                       int64_t &comp_size);

  static int generate_in_list(const ObIArray<int64_t> &list,
                              ObSqlString &sql_string);


  static int get_obj_str(const common::ObObj &obj,
                         common::ObIAllocator &allocator,
                         common::ObString &out_str,
                         const ObObjPrintParams &print_params);

  static int get_obj_binary_hex_str(const common::ObObj &obj,
                                    common::ObIAllocator &allocator,
                                    common::ObString &out_str);

  static int hex_str_to_obj(const char *buf,
                            int64_t buf_len,
                            common::ObIAllocator &allocator,
                            common::ObObj &obj);

  static int get_valid_obj_str(const common::ObObj &src_obj,
                               common::ObObjMeta dst_column_meta,
                               common::ObIAllocator &allocator,
                               common::ObString &dest_str,
                               const ObObjPrintParams &print_params);

  int fetch_table_rowcnt(const uint64_t tenant_id,
                         const uint64_t table_id,
                         const ObIArray<ObTabletID> &all_tablet_ids,
                         const ObIArray<share::ObLSID> &all_ls_ids,
                         ObIArray<ObOptTableStat> &tstats);

private:
  int get_table_stat_sql(const uint64_t tenant_id,
                         const ObOptTableStat &stat,
                         const int64_t current_time,
                         const bool is_index,
                         ObSqlString &sql_string);
  int get_table_stat_history_sql(const uint64_t tenant_id,
                                 const ObOptTableStat &stat,
                                 const int64_t saving_time,
                                 ObSqlString &sql_string);
  int get_column_stat_sql(const uint64_t tenant_id,
                                ObIAllocator &allocator,
                                const ObOptColumnStat &stat,
                                const int64_t current_time,
                                ObObjMeta min_meta,
                                ObObjMeta max_meta,
                                ObSqlString &sql_string,
                                const ObObjPrintParams &print_params);
  int get_column_stat_history_sql(const uint64_t tenant_id,
                                  ObIAllocator &allocator,
                                  const ObOptColumnStat &stat,
                                  const int64_t saving_time,
                                  ObObjMeta min_meta,
                                  ObObjMeta max_meta,
                                  ObSqlString &sql_string,
                                  const ObObjPrintParams &print_params);
  int get_histogram_stat_sql(const uint64_t tenant_id,
                             const ObOptColumnStat &stat,
                             common::ObIAllocator &allocator,
                             ObHistBucket &bucket,
                             ObObjMeta endpoint_meta,
                             ObSqlString &sql_string,
                             const ObObjPrintParams &print_params);
  int get_histogram_stat_history_sql(const uint64_t tenant_id,
                                     const ObOptColumnStat &stat,
                                     ObIAllocator &allocator,
                                     const ObHistBucket &bucket,
                                     const int64_t saving_time,
                                     ObObjMeta endpoint_meta,
                                     ObSqlString &sql_string,
                                     const ObObjPrintParams &print_params);

  int generate_in_list(const ObIArray<uint64_t> &list,
                       ObSqlString &sql_string);

  int construct_column_stat_sql(share::schema::ObSchemaGetterGuard *schema_guard,
                                const uint64_t tenant_id,
                                ObIAllocator &allocator,
                                const ObIArray<ObOptColumnStat*> &column_stats,
                                const int64_t current_time,
                                bool is_history_stat,
                                ObSqlString &column_stats_sql,
                                const ObObjPrintParams &print_params);

  int construct_delete_column_histogram_sql(const uint64_t tenant_id,
                                            const ObIArray<ObOptColumnStat*> &column_stats,
                                            ObSqlString &delete_histogram_sql);

  int construct_histogram_insert_sql(share::schema::ObSchemaGetterGuard *schema_guard,
                                     const uint64_t tenant_id,
                                     ObIAllocator &allocator,
                                     const ObIArray<ObOptColumnStat*> &column_stats,
                                     const int64_t current_time,
                                     bool is_history_stat,
                                     ObSqlString &insert_histogram_sql,
                                     bool &need_histogram,
                                     const ObObjPrintParams &print_params);

  int generate_specified_keys_list_str(const uint64_t tenant_id,
                                       ObIArray<ObOptKeyColumnStat> &key_col_stats,
                                       ObSqlString &keys_list_str);

  int generate_key_index_map(const uint64_t tenant_id,
                             ObIArray<ObOptKeyColumnStat> &key_col_stats,
                             hash::ObHashMap<ObOptKeyInfo, int64_t> &key_index_map);

  int fetch_histogram_stat(const uint64_t tenant_id,
                           ObIAllocator &allocator,
                           ObIArray<ObOptKeyColumnStat> &key_col_stats);

  int fill_bucket_stat(ObIAllocator &allocator,
                       sqlclient::ObMySQLResult &result,
                       hash::ObHashMap<ObOptKeyInfo, int64_t> &key_index_map,
                       ObIArray<ObOptKeyColumnStat> &key_col_stats);

  int get_column_stat_min_max_meta(share::schema::ObSchemaGetterGuard *schema_guard,
                                          const uint64_t tenant_id,
                                          const uint64_t table_id,
                                          ObObjMeta &min_meta,
                                          ObObjMeta &max_meta);

  int get_histogram_endpoint_meta(share::schema::ObSchemaGetterGuard *schema_guard,
                                  const uint64_t tenant_id,
                                  const uint64_t table_id,
                                  ObObjMeta &endpoint_meta);

  int gen_tablet_list_str(const ObIArray<ObTabletID> &all_tablet_ids,
                          const ObIArray<share::ObLSID> &all_ls_ids,
                          ObSqlString &tablet_list_str,
                          ObSqlString &tablet_ls_list_str);

  static const char *bitmap_compress_lib_name;

  bool inited_;
  ObMySQLProxy *mysql_proxy_;
  lib::ObMutex mutex_;
  ObServerConfig *config_;
};

}
}

#endif /* _OB_OPT_STAT_SQL_SERVICE_H_ */

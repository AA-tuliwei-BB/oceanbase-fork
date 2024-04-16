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

#include <stdio.h>
#include "lib/alloc/alloc_assist.h"
#include "ob_non_reserved_keywords.h"
#include "sql/parser/sql_parser_mysql_mode_tab.h"

static t_node *mysql_none_reserved_keywords_root = NULL;
static t_node *mysql_sql_keywords_in_pl_root = NULL;

/* List of non-reserved keywords */
/*一开始会对这些word建立一颗trie树，对于每次的查找来言，树是固定的
 *若新添加的keyword含有除大小写字母、'_'和数字以外的其它字符，请联系@叶提修改这颗树。
 *实现不需要保证字典序，但是原则上还是保证字典序，方便维护和查找*/
static const NonReservedKeyword Mysql_none_reserved_keywords[] =
{
  {"access", ACCESS},
  {"accessible", ACCESSIBLE},
  {"account", ACCOUNT},
  {"action", ACTION},
  {"activate", ACTIVATE},
  {"active", ACTIVE},
  {"add", ADD},
  {"adddate", ADDDATE},
  {"admin", ADMIN},
  {"after", AFTER},
  {"against", AGAINST},
  {"aggregate", AGGREGATE},
  {"algorithm", ALGORITHM},
  {"all", ALL},
  {"all_meta", ALL_META},
  {"all_user", ALL_USER},
  {"alter", ALTER},
  {"always", ALWAYS},
  {"analyse", ANALYSE},
  {"analyze", ANALYZE},
  {"and", AND},
  {"any", ANY},
  {"approx_count_distinct", APPROX_COUNT_DISTINCT},
  {"approx_count_distinct_synopsis", APPROX_COUNT_DISTINCT_SYNOPSIS},
  {"approx_count_distinct_synopsis_merge", APPROX_COUNT_DISTINCT_SYNOPSIS_MERGE},
  {"arbitration", ARBITRATION},
  {"archivelog", ARCHIVELOG},
  {"as", AS},
  {"asc", ASC},
  {"asensitive", ASENSITIVE},
  {"ascii", ASCII},
  {"at", AT},
  {"authors", AUTHORS},
  {"auto", AUTO},
  {"auto_increment", AUTO_INCREMENT},
  {"auto_increment_mode", AUTO_INCREMENT_MODE},
  {"auto_increment_cache_size", AUTO_INCREMENT_CACHE_SIZE},
  {"autoextend_size", AUTOEXTEND_SIZE},
  {"availability", AVAILABILITY},
  {"avg", AVG},
  {"avg_row_length", AVG_ROW_LENGTH},
  {"backup", BACKUP},
  {"backupset", BACKUPSET},
  {"balance", BALANCE},
  {"base", BASE},
  {"baseline", BASELINE},
  {"baseline_id", BASELINE_ID},
  {"basic", BASIC},
  {"before", BEFORE},
  {"begin", BEGI},
  {"between", BETWEEN},
  {"bigint", BIGINT},
  {"binary", BINARY},
  {"binlog", BINLOG},
  {"bit", BIT},
  {"bit_and", BIT_AND},
  {"bit_or", BIT_OR},
  {"bit_xor", BIT_XOR},
  {"blob", BLOB},
  {"block", BLOCK},
  {"block_size", BLOCK_SIZE},
  {"bool", BOOL},
  {"boolean", BOOLEAN},
  {"bootstrap", BOOTSTRAP},
  {"both", BOTH},
  {"breadth", BREADTH},
  {"btree", BTREE},
  {"by", BY},
  {"byte", BYTE},
  {"buckets", BUCKETS},
  {"backup_copies", BACKUP_COPIES},
  {"badfile", BADFILE},
  {"cache", CACHE},
  {"calibration", CALIBRATION},
  {"calibration_info", CALIBRATION_INFO},
  {"call", CALL},
  {"calc_partition_id", CALC_PARTITION_ID},
  {"cancel", CANCEL},
  {"cascaded", CASCADED},
  {"cascade", CASCADE},
  {"case", CASE},
  {"cast", CAST},
  {"catalog_name", CATALOG_NAME},
  {"chain", CHAIN},
  {"change", CHANGE},
  {"changed", CHANGED},
  {"char", CHARACTER},
  {"character", CHARACTER},
  {"charset", CHARSET},
  {"check", CHECK},
  {"checksum", CHECKSUM},
  {"checkpoint", CHECKPOINT},
  {"chunk", CHUNK},
  {"cipher", CIPHER},
  {"class_origin", CLASS_ORIGIN},
  {"clean", CLEAN},
  {"clear", CLEAR},
  {"client", CLIENT},
  {"close", CLOSE},
  {"cluster", CLUSTER},
  {"cluster_name", CLUSTER_NAME},
  {"cluster_id", CLUSTER_ID},
  {"coalesce", COALESCE},
  {"code", CODE},
  {"collate", COLLATE},
  {"collation", COLLATION},
  {"column_format", COLUMN_FORMAT},
  {"column_name", COLUMN_NAME},
  {"column", COLUMN},
  {"columns", COLUMNS},
  {"comment", COMMENT},
  {"commit", COMMIT},
  {"committed", COMMITTED},
  {"compact", COMPACT},
  {"completion", COMPLETION},
  {"compressed", COMPRESSED},
  {"compression", COMPRESSION},
  {"compute", COMPUTE},
  {"concurrent", CONCURRENT},
  {"condensed", CONDENSED},
  {"condition", CONDITION},
  {"connection", CONNECTION},
  {"consistent", CONSISTENT},
  {"constraint", CONSTRAINT},
  {"constraint_catalog", CONSTRAINT_CATALOG},
  {"constraint_name", CONSTRAINT_NAME},
  {"constraint_schema", CONSTRAINT_SCHEMA},
  {"contains", CONTAINS},
  {"context", CONTEXT},
  {"contributors", CONTRIBUTORS},
  {"continue", CONTINUE},
  {"connect", CONNECT},
  {"convert", CONVERT},
  {"copy", COPY},
  {"count", COUNT},
  {"cpu", CPU},
  {"create", CREATE},
  {"create_timestamp", CREATE_TIMESTAMP},
  {"cross", CROSS},
  {"ctxcat", CTXCAT},
  {"cube", CUBE},
  {"cume_dist", CUME_DIST},
  {"curdate", CURDATE},
  {"current", CURRENT},
  {"current_date", CURRENT_DATE},
  {"current_time", CURRENT_TIME},
  {"current_timestamp", CURRENT_TIMESTAMP},
  {"current_user", CURRENT_USER},
  {"cursor", CURSOR},
  {"cursor_name", CURSOR_NAME},
  {"curtime", CURTIME},
  {"ctx_id", CTX_ID},
  {"cycle", CYCLE},
  {"dag", DAG},
  {"data", DATA},
  {"database_id", DATABASE_ID},
  {"datafile", DATAFILE},
  {"data_table_id", DATA_TABLE_ID},
  {"database", DATABASE},
  {"databases", DATABASES},
  {"data_source", DATA_SOURCE},
  {"date", DATE},
  {"date_add", DATE_ADD},
  {"date_sub", DATE_SUB},
  {"datetime", DATETIME},
  {"day", DAY},
  {"day_hour", DAY_HOUR},
  {"day_microsecond", DAY_MICROSECOND},
  {"day_minute", DAY_MINUTE},
  {"day_second", DAY_SECOND},
  {"deallocate", DEALLOCATE},
  {"dec", NUMBER},
  {"decimal", DECIMAL},
  {"declare", DECLARE},
  {"decryption", DECRYPTION},
  {"default", DEFAULT},
  {"default_auth", DEFAULT_AUTH},
  {"default_lob_inrow_threshold", DEFAULT_LOB_INROW_THRESHOLD},
  {"definer", DEFINER},
  {"delay", DELAY},
  {"delayed", DELAYED},
  {"delay_key_write", DELAY_KEY_WRITE},
  {"delete", DELETE},
  {"depth", DEPTH},
  {"des_key_file", DES_KEY_FILE},
  {"destination", DESTINATION},
  {"desc", DESC},
  {"describe", DESCRIBE},
  {"description", DESCRIPTION},
  {"deterministic", DETERMINISTIC},
  {"dense_rank", DENSE_RANK},
  {"diagnostics", DIAGNOSTICS},
  {"disconnect", DISCONNECT},
  {"directory", DIRECTORY},
  {"disable", DISABLE},
  {"discard", DISCARD},
  {"disk", DISK},
  {"diskgroup", DISKGROUP},
  {"distinct", DISTINCT},
  {"distinctrow", DISTINCT},
  {"div", DIV},
  {"do", DO},
  {"double", DOUBLE},
  {"drop", DROP},
  {"dual", DUAL},
  {"dump", DUMP},
  {"dumpfile", DUMPFILE},
  {"duplicate", DUPLICATE},
  {"dynamic", DYNAMIC},
  {"default_tablegroup", DEFAULT_TABLEGROUP},
  {"each", EACH},
  {"effective", EFFECTIVE},
  {"empty", EMPTY},
  {"else", ELSE},
  {"elseif", ELSEIF},
  {"enable", ENABLE},
  {"enable_arbitration_service", ENABLE_ARBITRATION_SERVICE},
  {"enable_extended_rowid", ENABLE_EXTENDED_ROWID},
  {"enclosed", ENCLOSED},
  {"encoding", ENCODING},
  {"encrypted", ENCRYPTED},
  {"encryption", ENCRYPTION},
  {"end", END},
  {"ends", ENDS},
  {"enforced", ENFORCED},
  {"engine", ENGINE_},
  {"engines", ENGINES},
  {"enum", ENUM},
  {"empty_field_as_null", EMPTY_FIELD_AS_NULL},
  {"entity", ENTITY},
  {"error", ERROR_P},
  {"error_code", ERROR_CODE},
  {"errors", ERRORS},
  {"escape", ESCAPE},
  {"escaped", ESCAPED},
  {"estimate", ESTIMATE},
  {"event", EVENT},
  {"events", EVENTS},
  {"every", EVERY},
  {"except", EXCEPT},
  {"exchange", EXCHANGE},
  {"execute", EXECUTE},
  {"exists", EXISTS},
  {"exit", EXIT},
  {"expansion", EXPANSION},
  {"explain", EXPLAIN},
  {"expire", EXPIRE},
  {"expired", EXPIRED},
  {"expire_info", EXPIRE_INFO},
  {"export", EXPORT},
  {"extended", EXTENDED},
  {"extended_noaddr", EXTENDED_NOADDR},
  {"extent_size", EXTENT_SIZE},
  {"external", EXTERNAL},
  {"extract", EXTRACT},
  {"failover", FAILOVER},
  {"fast", FAST},
  {"faults", FAULTS},
  {"fetch", FETCH},
  {"fields", FIELDS},
  {"field_optionally_enclosed_by", FIELD_OPTIONALLY_ENCLOSED_BY},
  {"field_delimiter", FIELD_DELIMITER},
  {"file", FILEX},
  {"file_id", FILE_ID},
  {"final_count", FINAL_COUNT},
  {"first", FIRST},
  {"first_value", FIRST_VALUE},
  {"fixed", FIXED},
  {"flashback", FLASHBACK},
  {"float", FLOAT},
  {"float4", FLOAT},
  {"float8", DOUBLE},
  {"flush", FLUSH},
  {"follower", FOLLOWER},
  {"for", FOR},
  {"force", FORCE},
  {"foreign", FOREIGN},
  {"format", FORMAT},
  {"found", FOUND},
  {"fragmentation", FRAGMENTATION},
  {"freeze", FREEZE},
  {"frequency", FREQUENCY},
  {"from", FROM},
  {"frozen", FROZEN},
  {"full", FULL},
  {"fulltext", FULLTEXT},
  {"function", FUNCTION},
  {"following", FOLLOWING},
  {"general", GENERAL},
  {"generated", GENERATED},
  {"geometry", GEOMETRY},
  {"geomcollection", GEOMCOLLECTION},
  {"geometrycollection", GEOMETRYCOLLECTION},
  {"get", GET},
  {"get_format", GET_FORMAT},
  {"global", GLOBAL},
  {"global_name", GLOBAL_NAME},
  {"grant", GRANT},
  {"grants", GRANTS},
  {"group", GROUP},
  {"grouping", GROUPING},
  {"group_concat", GROUP_CONCAT},
  {"gts", GTS},
  {"handler", HANDLER},
  {"having", HAVING},
  {"hash", HASH},
  {"help", HELP},
  {"high_priority", HIGH_PRIORITY},
  {"histogram", HISTOGRAM},
  {"host", HOST},
  {"hosts", HOSTS},
  {"hour", HOUR},
  {"hour_microsecond", HOUR_MICROSECOND},
  {"hour_minute", HOUR_MINUTE},
  {"hour_second", HOUR_SECOND},
  {"hidden", HIDDEN},
  {"hybrid_hist", HYBRID_HIST},
  {"id", ID},
  {"idc", IDC},
  {"identified", IDENTIFIED},
  {"if", IF},
  {"ignore", IGNORE},
  {"ignore_server_ids", IGNORE_SERVER_IDS},
  {"import", IMPORT},
  {"in", IN},
  {"incr", INCR},
  {"increment", INCREMENT},
  {"incremental", INCREMENTAL},
  {"index", INDEX},
  {"indexes", INDEXES},
  {"index_table_id", INDEX_TABLE_ID},
  {"infile", INFILE},
  {"info", INFO},
  {"initial_size", INITIAL_SIZE},
  {"inner", INNER},
  {"innodb", INNODB},
  {"inout", INOUT},
  {"insensitive", INSENSITIVE},
  {"insert_method", INSERT_METHOD},
  {"install", INSTALL},
  {"instance", INSTANCE},
  {"int", INTEGER},
  {"int1", TINYINT},
  {"int2", SMALLINT},
  {"int3", MEDIUMINT},
  {"int4", INTEGER},
  {"int8", BIGINT},
  {"insert", INSERT},
  {"integer", INTEGER},
  {"into", INTO},
  {"intersect", INTERSECT},
  {"invisible", INVISIBLE},
  {"invoker", INVOKER},
  {"io", IO},
  {"iops_weight", IOPS_WEIGHT},
  {"io_after_gtids", IO_AFTER_GTIDS},
  {"io_before_gtids", IO_BEFORE_GTIDS},
  {"io_thread", IO_THREAD},
  {"ipc", IPC},
  {"is", IS},
  {"issuer", ISSUER},
  {"isnull", ISNULL},
  {"isolate", ISOLATE},
  {"isolation", ISOLATION},
  {"issuer", ISSUER},
  {"iterate", ITERATE},
  {"job", JOB},
  {"join", JOIN},
  {"json", JSON},
  {"json_arrayagg", JSON_ARRAYAGG},
  {"json_objectagg", JSON_OBJECTAGG},
  {"json_value", JSON_VALUE},
  {"json_table", JSON_TABLE},
  {"key", KEY},
  {"keys", KEYS},
  {"key_block_size", KEY_BLOCK_SIZE},
  {"key_version", KEY_VERSION},
  {"kill", KILL},
  {"kvcache", KVCACHE},
  {"ilogcache", ILOGCACHE},
  {"indexed", INDEXED},
  {"file_id", FILE_ID},
  {"lag", LAG},
  {"language", LANGUAGE},
  {"last", LAST},
  {"last_value", LAST_VALUE},
  {"lateral", LATERAL},
  {"lead", LEAD},
  {"leader", LEADER},
  {"leading", LEADING},
  {"leave", LEAVE},
  {"leaves", LEAVES},
  {"leak", LEAK},
  {"leak_mod", LEAK_MOD},
  {"leak_rate", LEAK_RATE},
  {"left", LEFT},
  {"less", LESS},
  {"level", LEVEL},
  {"link", LINK},
  {"lib", LIB},
  {"like", LIKE},
  {"limit", LIMIT},
  {"linear", LINEAR},
  {"lines", LINES},
  {"linestring", LINESTRING},
  {"line_delimiter", LINE_DELIMITER},
  {"list", BISON_LIST},
  {"listagg", LISTAGG},
  {"load", LOAD},
  {"ln", LN},
  {"lob_inrow_threshold", LOB_INROW_THRESHOLD},
  {"local", LOCAL},
  {"locality", LOCALITY},
  {"localtime", LOCALTIME},
  {"localtimestamp", LOCALTIMESTAMP},
  {"location", LOCATION},
  {"lock", LOCK_},
  {"locked", LOCKED},
  {"locks", LOCKS},
  {"logfile", LOGFILE},
  {"logonly_replica_num", LOGONLY_REPLICA_NUM},
  {"log", LOG},
  {"logs", LOGS},
  {"log_restore_source", LOG_RESTORE_SOURCE},
  {"long", MEDIUMTEXT},
  {"longblob", LONGBLOB},
  {"longtext", LONGTEXT},
  {"loop", LOOP},
  {"low_priority", LOW_PRIORITY},
  {"ls", LS},
  {"major", MAJOR},
  {"manual", MANUAL},
  {"master", MASTER},
  {"master_bind", MASTER_BIND},
  {"master_auto_position", MASTER_AUTO_POSITION},
  {"master_connect_retry", MASTER_CONNECT_RETRY},
  {"master_delay", MASTER_DELAY},
  {"master_heartbeat_period", MASTER_HEARTBEAT_PERIOD},
  {"master_host", MASTER_HOST},
  {"master_log_file", MASTER_LOG_FILE},
  {"master_log_pos", MASTER_LOG_POS},
  {"master_password", MASTER_PASSWORD},
  {"master_port", MASTER_PORT},
  {"master_retry_count", MASTER_RETRY_COUNT},
  {"master_server_id", MASTER_SERVER_ID},
  {"master_ssl", MASTER_SSL},
  {"master_ssl_ca", MASTER_SSL_CA},
  {"master_ssl_capath", MASTER_SSL_CAPATH},
  {"master_ssl_cert", MASTER_SSL_CERT},
  {"master_ssl_cipher", MASTER_SSL_CIPHER},
  {"master_ssl_crl", MASTER_SSL_CRL},
  {"master_ssl_crlpath", MASTER_SSL_CRLPATH},
  {"master_ssl_key", MASTER_SSL_KEY},
  {"master_ssl_verify_server_cert", MASTER_SSL_VERIFY_SERVER_CERT},
  {"master_user", MASTER_USER},
  {"match", MATCH},
  {"max", MAX},
  {"maxvalue", MAXVALUE},
  {"maximize", MAXIMIZE},
  {"max_connections_per_hour", MAX_CONNECTIONS_PER_HOUR},
  {"max_cpu", MAX_CPU},
  {"log_disk_size", LOG_DISK_SIZE},
  {"max_iops", MAX_IOPS},
  {"memory_size", MEMORY_SIZE},
  {"max_queries_per_hour", MAX_QUERIES_PER_HOUR},
  {"max_rows", MAX_ROWS},
  {"max_size", MAX_SIZE},
  {"max_updates_per_hour", MAX_UPDATES_PER_HOUR},
  {"max_used_part_id", MAX_USED_PART_ID},
  {"max_user_connections", MAX_USER_CONNECTIONS},
  {"medium", MEDIUM},
  {"member", MEMBER},
  {"mediumblob", MEDIUMBLOB},
  {"mediumint", MEDIUMINT},
  {"mediumtext", MEDIUMTEXT},
  {"memory", MEMORY},
  {"memstore_percent", MEMSTORE_PERCENT},
  {"memtable", MEMTABLE},
  {"merge", MERGE},
  {"message_text", MESSAGE_TEXT},
  {"meta", META},
  {"microsecond", MICROSECOND},
  {"middleint", MEDIUMINT},
  {"migrate", MIGRATE},
  {"migration", MIGRATION},
  {"min", MIN},
  {"minvalue", MINVALUE},
  {"min_cpu", MIN_CPU},
  {"min_iops", MIN_IOPS},
  {"min_rows", MIN_ROWS},
  {"minor", MINOR},
  {"minute", MINUTE},
  {"minute_microsecond", MINUTE_MICROSECOND},
  {"minute_second", MINUTE_SECOND},
  {"minus", MINUS},
  {"mod", MOD},
  {"mode", MODE},
  {"modify", MODIFY},
  {"modifies", MODIFIES},
  {"month", MONTH},
  {"move", MOVE},
  {"multilinestring", MULTILINESTRING},
  {"multipoint", MULTIPOINT},
  {"multipolygon", MULTIPOLYGON},
  {"mutex", MUTEX},
  {"mysql_errno", MYSQL_ERRNO},
  {"my_name", MY_NAME},
  {"name", NAME},
  {"names", NAMES},
  {"namespace", NAMESPACE},
  {"national", NATIONAL},
  {"natural", NATURAL},
  {"nchar", NCHAR},
  {"ndb", NDB},
  {"ndbcluster", NDBCLUSTER},
  {"nested", NESTED},
  {"new", NEW},
  {"next", NEXT},
  {"no", NO},
  {"no_write_to_binlog", NO_WRITE_TO_BINLOG},
  {"noarchivelog", NOARCHIVELOG},
  {"noaudit", NOAUDIT},
  {"nocache", NOCACHE},
  {"nocycle", NOCYCLE},
  {"nomaxvalue", NOMAXVALUE},
  {"nominvalue", NOMINVALUE},
  {"noorder", NOORDER},
  {"no_parallel", NO_PARALLEL},
  {"no_rewrite", NO_REWRITE},
  {"no_wait", NO_WAIT},
  {"nodegroup", NODEGROUP},
  {"none", NONE},
  {"noparallel", NOPARALLEL},
  {"normal", NORMAL},
  {"now", NOW},
  {"nowait", NOWAIT},
  {"nulls", NULLS},
  {"null_if", NULL_IF_EXETERNAL},
  {"numeric", DECIMAL},
  {"number", NUMBER},
  {"nvarchar", NVARCHAR},
  {"occur", OCCUR},
  {"ntile", NTILE},
  {"nth_value", NTH_VALUE},
  {"obconfig_url", OBCONFIG_URL},
  {"object_id", OBJECT_ID},
  {"of", OF},
  {"off", OFF},
  {"offset", OFFSET},
  {"old", OLD},
  {"old_key", OLD_KEY},
  {"oj", OJ},
  {"over", OVER},
  {"old_password", OLD_PASSWORD},
  {"on", ON},
  {"one", ONE},
  {"one_shot", ONE_SHOT},
  {"only", ONLY},
  {"open", OPEN},
  {"optimize", OPTIMIZE},
  {"option", OPTION},
  {"optionally", OPTIONALLY},
  {"options", OPTIONS},
  {"or", OR},
  {"order", ORDER},
  {"ordinality", ORDINALITY},
  {"orig_default", ORIG_DEFAULT},
  {"out", OUT},
  {"outer", OUTER},
  {"outfile", OUTFILE},
  {"outline", OUTLINE},
  {"owner", OWNER},
  {"pack_keys", PACK_KEYS},
  {"page", PAGE},
  {"parameters", PARAMETERS},
  {"parallel", PARALLEL},
  {"parser", PARSER},
  {"partial", PARTIAL},
  {"partition", PARTITION},
  {"partition_id", PARTITION_ID},
  {"partitioning", PARTITIONING},
  {"partitions", PARTITIONS},
  {"password", PASSWORD},
  {"path", PATH},
  {"pattern", PATTERN},
  {"pause", PAUSE},
  {"paxos_replica_num", PAXOS_REPLICA_NUM},
  {"percentage", PERCENTAGE},
  {"percent_rank", PERCENT_RANK},
  {"performance", PERFORMANCE},
  {"phase", PHASE},
  {"physical", PHYSICAL},
  {"plan", PLAN},
  {"planregress", PLANREGRESS},
  {"plugin", PLUGIN},
  {"plugin_dir", PLUGIN_DIR},
  {"plugins", PLUGINS},
  {"plus", PLUS},
  {"point", POINT},
  {"policy", POLICY},
  {"polygon", POLYGON},
  {"pool", POOL},
  {"port", PORT},
  {"position", POSITION},
  {"precision", PRECISION},
  {"prepare", PREPARE},
  {"preserve", PRESERVE},
  {"prev", PREV},
  {"primary", PRIMARY},
  {"primary_zone", PRIMARY_ZONE},
  {"privileges", PRIVILEGES},
  {"procedure", PROCEDURE},
  {"process", PROCESS},
  {"processlist", PROCESSLIST},
  {"profile", PROFILE},
  {"profiles", PROFILES},
  {"progressive_merge_num", PROGRESSIVE_MERGE_NUM},
  {"protection", PROTECTION},
  {"proxy", PROXY},
  {"public", PUBLIC},
  {"purge", PURGE},
  {"p_entity", P_ENTITY},
  {"p_chunk", P_CHUNK},
  {"preceding", PRECEDING},
  {"pctfree", PCTFREE},
  {"ps", PS},
  {"quarter", QUARTER},
  {"query", QUERY},
  {"quick", QUICK},
  {"range", RANGE},
  {"rank", RANK},
  {"read", READ},
  {"read_write", READ_WRITE},
  {"read_consistency", READ_CONSISTENCY},
  {"read_only", READ_ONLY},
  {"reads", READS},
  {"real", REAL},
  {"rebuild", REBUILD},
  {"recover", RECOVER},
  {"recovery", RECOVERY},
  {"recovery_window", RECOVERY_WINDOW},
  {"recycle", RECYCLE},
  {"recyclebin", RECYCLEBIN},
  {"redo_buffer_size", REDO_BUFFER_SIZE},
  {"redo_transport_options", REDO_TRANSPORT_OPTIONS},
  {"redofile", REDOFILE},
  {"redundancy", REDUNDANCY},
  {"redundant", REDUNDANT},
  {"references", REFERENCES},
  {"refresh", REFRESH},
  {"regexp", REGEXP},
  {"region", REGION},
  {"reject", REJECT},
  {"relay", RELAY},
  {"relay_log_file", RELAY_LOG_FILE},
  {"relay_log_pos", RELAY_LOG_POS},
  {"relay_thread", RELAY_THREAD},
  {"relaylog", RELAYLOG},
  {"release", RELEASE},
  {"reload", RELOAD},
  {"remap", REMAP},
  {"remove", REMOVE},
  {"rename", RENAME},
  {"reorganize", REORGANIZE},
  {"repair", REPAIR},
  {"repeat", REPEAT},
  {"repeatable", REPEATABLE},
  {"replace", REPLACE},
  {"replica", REPLICA},
  {"replica_num", REPLICA_NUM},
  {"replica_type", REPLICA_TYPE},
  {"duplicate_scope", DUPLICATE_SCOPE},
  {"replication", REPLICATION},
  {"report", REPORT},
  {"require", REQUIRE},
  {"reset", RESET},
  {"resignal", RESIGNAL},
  {"resource", RESOURCE},
  {"resource_pool_list", RESOURCE_POOL_LIST},
  {"respect", RESPECT},
  {"restart", RESTART},
  {"restore", RESTORE},
  {"restrict", RESTRICT},
  {"resume", RESUME},
  {"returning", RETURNING},
  {"returned_sqlstate", RETURNED_SQLSTATE},
  {"return", RETURN},
  {"returns", RETURNS},
  {"reverse", REVERSE},
  {"revoke", REVOKE},
  {"right", RIGHT},
  {"rlike", REGEXP},
  {"role", ROLE},
  {"rollback", ROLLBACK},
  {"rolling", ROLLING},
  {"rollup", ROLLUP},
  {"root", ROOT},
  {"rootservice", ROOTSERVICE},
  {"rootservice_list", ROOTSERVICE_LIST},
  {"roottable", ROOTTABLE},
  {"rotate", ROTATE},
  {"routine", ROUTINE},
  {"row", ROW},
  {"row_count", ROW_COUNT},
  {"row_format", ROW_FORMAT},
  {"row_number", ROW_NUMBER},
  {"rows", ROWS},
  {"rtree", RTREE},
  {"run", RUN},
  {"sample", SAMPLE},
  {"savepoint", SAVEPOINT},
  {"schedule", SCHEDULE},
  {"schema_name", SCHEMA_NAME},
  {"scn", SCN},
  {"scope", SCOPE},
  {"second", SECOND},
  {"second_microsecond", SECOND_MICROSECOND},
  {"security", SECURITY},
  {"seed", SEED},
  {"sensitive", SENSITIVE},
  {"sequence", SEQUENCE},
  {"sequences", SEQUENCES},
  {"serial", SERIAL},
  {"serializable", SERIALIZABLE},
  {"server", SERVER},
  {"server_ip", SERVER_IP},
  {"server_port", SERVER_PORT},
  {"server_type", SERVER_TYPE},
  {"service", SERVICE},
  {"set", SET},
  {"session", SESSION},
  {"session_user", SESSION_USER},
  {"skip_header", SKIP_HEADER},
  {"skip_blank_lines", SKIP_BLANK_LINES},
  {"statements", STATEMENTS},
  {"statistics", STATISTICS},
  {"binding", BINDING},
  {"sharding", SHARDING},
  {"schema", SCHEMA},
  {"schemas", SCHEMAS},
  {"select", SELECT},
  {"set_master_cluster", SET_MASTER_CLUSTER},
  {"set_slave_cluster", SET_SLAVE_CLUSTER},
  {"set_tp", SET_TP},
  {"separator", SEPARATOR},
  {"share", SHARE},
  {"show", SHOW},
  {"skewonly", SKEWONLY},
  {"shutdown", SHUTDOWN},
  {"signed", SIGNED},
  {"signal", SIGNAL},
  {"simple", SIMPLE},
  {"slave", SLAVE},
  {"size", SIZE},
  {"slog", SLOG},
  {"slow", SLOW},
  {"slot_idx", SLOT_IDX},
  {"smallint", SMALLINT},
  {"snapshot", SNAPSHOT},
  {"socket", SOCKET},
  {"some", SOME},
  {"soname", SONAME},
  {"sounds", SOUNDS},
  {"source", SOURCE},
  {"spatial", SPATIAL},
  {"specific", SPECIFIC},
  {"spfile", SPFILE},
  {"split", SPLIT},
  {"sql", SQL},
  {"sqlexception", SQLEXCEPTION},
  {"sqlstate", SQLSTATE},
  {"sqlwarning", SQLWARNING},
  {"sql_big_result", SQL_BIG_RESULT},
  {"sql_calc_found_rows", SQL_CALC_FOUND_ROWS},
  {"sql_small_result", SQL_SMALL_RESULT},
  {"sql_after_gtids", SQL_AFTER_GTIDS},
  {"sql_after_mts_gaps", SQL_AFTER_MTS_GAPS},
  {"sql_before_gtids", SQL_BEFORE_GTIDS},
  {"sql_buffer_result", SQL_BUFFER_RESULT},
  {"sql_cache", SQL_CACHE},
  {"sql_id", SQL_ID},
  {"schema_id", SCHEMA_ID},
  {"sql_no_cache", SQL_NO_CACHE},
  {"sql_thread", SQL_THREAD},
  {"sql_tsi_day", SQL_TSI_DAY},
  {"sql_tsi_hour", SQL_TSI_HOUR},
  {"sql_tsi_minute", SQL_TSI_MINUTE},
  {"sql_tsi_month", SQL_TSI_MONTH},
  {"sql_tsi_quarter", SQL_TSI_QUARTER},
  {"sql_tsi_second", SQL_TSI_SECOND},
  {"sql_tsi_week", SQL_TSI_WEEK},
  {"sql_tsi_year", SQL_TSI_YEAR},
  {"srid", SRID},
  {"ssl", SSL},
  {"stacked", STACKED},
  {"standby", STANDBY},
  {"start", START},
  {"starts", STARTS},
  {"starting", STARTING},
  {"straight_join", STRAIGHT_JOIN},
  {"stat", STAT},
  {"stats_auto_recalc", STATS_AUTO_RECALC},
  {"stats_persistent", STATS_PERSISTENT},
  {"stats_sample_pages", STATS_SAMPLE_PAGES},
  {"status", STATUS},
  {"std", STD},
  {"stddev", STDDEV},
  {"stddev_pop", STDDEV_POP},
  {"stddev_samp", STDDEV_SAMP},
  {"stop", STOP},
  {"storage", STORAGE},
  {"storage_format_version", STORAGE_FORMAT_VERSION},
  {"store", STORE},
  {"stored", STORED},
  {"storing", STORING},
  {"string", STRING},
  {"strong", STRONG},
  {"subclass_origin", SUBCLASS_ORIGIN},
  {"subdate", SUBDATE},
  {"subject", SUBJECT},
  {"subpartition", SUBPARTITION},
  {"subpartitions", SUBPARTITIONS},
  {"substr", SUBSTR},
  {"substring", SUBSTRING},
  {"sum", SUM},
  {"super", SUPER},
  {"suspend", SUSPEND},
  {"successful", SUCCESSFUL},
  {"synchronization", SYNCHRONIZATION},
  {"sysdate", SYSDATE},
  {"system", SYSTEM},
  {"system_user", SYSTEM_USER},
  {"swaps", SWAPS},
  {"switch", SWITCH},
  {"switches", SWITCHES},
  {"switchover", SWITCHOVER},
  {"table", TABLE},
  {"table_checksum", TABLE_CHECKSUM},
  {"table_mode", TABLE_MODE},
  {"table_id", TABLE_ID},
  {"table_name", TABLE_NAME},
  {"tablegroup", TABLEGROUP},
  {"tablegroups", TABLEGROUPS},
  {"tablegroup_id", TABLEGROUP_ID},
  {"tables", TABLES},
  {"tablespace", TABLESPACE},
  {"tablet", TABLET},
  {"tablet_id", TABLET_ID},
  {"tablet_max_size", TABLET_MAX_SIZE},
  {"task", TASK},
  {"task_id", TASK_ID},
  {"template", TEMPLATE},
  {"temporary", TEMPORARY},
  {"temptable", TEMPTABLE},
  {"tenant", TENANT},
  {"tenant_id", TENANT_ID},
  {"terminated", TERMINATED},
  {"text", TEXT},
  {"than", THAN},
  {"then", THEN},
  {"time", TIME},
  {"timestamp", TIMESTAMP},
  {"timestampadd", TIMESTAMPADD},
  {"timestampdiff", TIMESTAMPDIFF},
  {"tinyblob", TINYBLOB},
  {"tinyint", TINYINT},
  {"tinytext", TINYTEXT},
  {"tablet_size", TABLET_SIZE},
  {"tp_name", TP_NAME},
  {"tp_no", TP_NO},
  {"trace", TRACE},
  {"traditional", TRADITIONAL},
  {"trailing", TRAILING},
  {"transfer", TRANSFER},
  {"transaction", TRANSACTION},
  {"trigger", TRIGGER},
  {"triggers", TRIGGERS},
  {"trim", TRIM},
  {"trim_space", TRIM_SPACE},
  {"truncate", TRUNCATE},
  {"type", TYPE},
  {"types", TYPES},
  {"to", TO},
  {"top_k_fre_hist", TOP_K_FRE_HIST},
  {"uncommitted", UNCOMMITTED},
  {"undefined", UNDEFINED},
  {"undo", UNDO},
  {"undo_buffer_size", UNDO_BUFFER_SIZE},
  {"undofile", UNDOFILE},
  {"union", UNION},
  {"unique", UNIQUE},
  {"unicode", UNICODE},
  {"uninstall", UNINSTALL},
  {"unit", UNIT},
  {"unit_group", UNIT_GROUP},
  {"unit_num", UNIT_NUM},
  {"unknown", UNKNOWN},
  {"unlock", UNLOCK},
  {"unlocked", UNLOCKED},
  {"unsigned", UNSIGNED},
  {"until", UNTIL},
  {"unusual", UNUSUAL},
  {"update", UPDATE},
  {"upgrade", UPGRADE},
  {"usage", USAGE},
  {"use", USE},
  {"using", USING},
  {"use_bloom_filter", USE_BLOOM_FILTER},
  {"use_frm", USE_FRM},
  {"user", USER},
  {"user_resources", USER_RESOURCES},
  {"utc_date", UTC_DATE},
  {"utc_time", UTC_TIME},
  {"utc_timestamp", UTC_TIMESTAMP},
  {"unbounded", UNBOUNDED},
  {"unlimited", UNLIMITED},
  {"valid", VALID},
  {"validate", VALIDATE},
  {"value", VALUE},
  {"varbinary", VARBINARY},
  {"varchar", VARCHAR},
  {"varcharacter", VARCHAR},
  {"variance", VARIANCE},
  {"variables", VARIABLES},
  {"var_pop", VAR_POP},
  {"var_samp", VAR_SAMP},
  {"verbose", VERBOSE},
  {"verify", VERIFY},
  {"materialized", MATERIALIZED},
  {"validate", VALIDATE},
  {"values", VALUES},
  {"varying", VARYING},
  {"view", VIEW},
  {"virtual", VIRTUAL},
  {"virtual_column_id", VIRTUAL_COLUMN_ID},
  {"visible", VISIBLE},
  {"wait", WAIT},
  {"warnings", WARNINGS},
  {"weak", WEAK},
  {"week", WEEK},
  {"weight_string", WEIGHT_STRING},
  {"where", WHERE},
  {"when", WHEN},
  {"whenever", WHENEVER},
  {"while", WHILE},
  {"window", WINDOW},
  {"with", WITH},
  {"work", WORK},
  {"write", WRITE},
  {"wrapper", WRAPPER},
  {"x509", X509},
  {"xa", XA},
  {"xml", XML},
  {"xor", XOR},
  {"year", YEAR},
  {"year_month", YEAR_MONTH},
  {"zone", ZONE},
  {"zone_list", ZONE_LIST},
  {"time_zone_info", TIME_ZONE_INFO},
  {"zone_type", ZONE_TYPE},
  {"zerofill", ZEROFILL},
  {"audit", AUDIT},
  {"PL", PL},
  {"remote_oss", REMOTE_OSS},
  {"throttle", THROTTLE},
  {"priority", PRIORITY},
  {"rt", RT},
  {"network", NETWORK},
  {"logical_reads", LOGICAL_READS},
  {"queue_time", QUEUE_TIME},
  {"obsolete", OBSOLETE},
  {"bandwidth", BANDWIDTH},
  {"backuppiece", BACKUPPIECE},
  {"backup_backup_dest", BACKUP_BACKUP_DEST},
  {"backed", BACKED},
  {"pretty", PRETTY},
  {"pretty_color", PRETTY_COLOR},
  {"preview", PREVIEW},
  {"up", UP},
  {"times", TIMES},
  {"backupround", BACKUPROUND},
  {"recursive", RECURSIVE},
  {"wash", WASH},
  {"query_response_time",QUERY_RESPONSE_TIME},
  {"statement_id", STATEMENT_ID},
  {"TTL", TTL},
  {"kv_attributes", KV_ATTRIBUTES},
  {"_st_asmvt", _ST_ASMVT},
};

/** https://dev.mysql.com/doc/refman/5.7/en/sql-syntax-prepared-statements.html
 ALTER TABLE
 ALTER USER
 ANALYZE TABLE
 CACHE INDEX
 CALL
 CHANGE MASTER
 CHECKSUM {TABLE | TABLES}
 COMMIT
 {CREATE | DROP} INDEX
 {CREATE | RENAME | DROP} DATABASE
 {CREATE | DROP} TABLE
 {CREATE | RENAME | DROP} USER
 {CREATE | DROP} VIEW
 DELETE
 DO
 FLUSH {TABLE | TABLES | TABLES WITH READ LOCK | HOSTS | PRIVILEGES
   | LOGS | STATUS | MASTER | SLAVE | DES_KEY_FILE | USER_RESOURCES}
 GRANT
 INSERT
 INSTALL PLUGIN
 KILL
 \LOAD INDEX INTO CACHE
 OPTIMIZE TABLE
 RENAME TABLE
 REPAIR TABLE
 REPLACE
 RESET {MASTER | SLAVE | QUERY CACHE}
 REVOKE
 SELECT
 SET
 SHOW {WARNINGS | ERRORS}
 SHOW BINLOG EVENTS
 SHOW CREATE {PROCEDURE | FUNCTION | EVENT | TABLE | VIEW}
 SHOW {MASTER | BINARY} LOGS
 SHOW {MASTER | SLAVE} STATUS
 SLAVE {START | STOP}
 TRUNCATE TABLE
 UNINSTALL PLUGIN
 UPDATE
 */
static const NonReservedKeyword Mysql_sql_keywords_in_pl[] =
{
  {"alter", ALTER},
  {"analyze", ANALYZE},
  {"binary", BINARY},
  {"binlog", BINLOG},
  {"cache", CACHE},
  {"call", CALL},
  {"change", CHANGE},
  {"checksum", CHECKSUM},
  {"commit", COMMIT},
  {"create", CREATE},
  {"database", DATABASE},
  {"delete", DELETE},
  {"des_key_file", DES_KEY_FILE},
  {"do", DO},
  {"drop", DROP},
  {"errors", ERRORS},
  {"event", EVENT},
  {"events", EVENTS},
  {"flush", FLUSH},
  {"function", FUNCTION},
  {"get", GET},
  {"grant", GRANT},
  {"index", INDEX},
  {"insert", INSERT},
  {"install", INSTALL},
  {"into", INTO},
  {"kill", KILL},
  {"load", LOAD},
  {"lock", LOCK_},
  {"logs", LOGS},
  {"master", MASTER},
  {"optimize", OPTIMIZE},
  {"plugin", PLUGIN},
  {"privileges", PRIVILEGES},
  {"procedure", PROCEDURE},
  {"query", QUERY},
  {"read", READ},
  {"rename", RENAME},
  {"repair", REPAIR},
  {"replace", REPLACE},
  {"reset", RESET},
  {"revoke", REVOKE},
  {"select", SELECT},
  {"set", SET},
  {"show", SHOW},
  {"slave", SLAVE},
  {"start", START},
  {"status", STATUS},
  {"stop", STOP},
  {"table", TABLE},
  {"tables", TABLES},
  {"truncate", TRUNCATE},
  {"uninstall", UNINSTALL},
  {"update", UPDATE},
  {"user_resources", USER_RESOURCES},
  {"view", VIEW},
  {"warnings", WARNINGS},
  {"with", WITH},
  /*
   * 其他在PL中出现的SQL关键字
   */
  {"prepare", PREPARE},
  {"execute", EXECUTE},
  {"deallocate", DEALLOCATE},
  {"unique", UNIQUE},
  {"temporary", TEMPORARY},
  {"savepoint", SAVEPOINT},
  {"rollback", ROLLBACK},
  {"desc", DESC},
  {"describe", DESCRIBE},
  {"explain", EXPLAIN},
  {"release", RELEASE},
};

const NonReservedKeyword *mysql_non_reserved_keyword_lookup(const char *word)
{
  return find_word(word, mysql_none_reserved_keywords_root, Mysql_none_reserved_keywords);
}

int mysql_sql_reserved_keyword_lookup(const char *word)
{
  int ret = -1;
  const NonReservedKeyword *res_word = find_word(word, mysql_sql_keywords_in_pl_root, Mysql_sql_keywords_in_pl);
  if (NULL != res_word) {
    ret = res_word->keyword_type;
  }
  return ret;
}

//return 0 if succ, return 1 if fail
int create_mysql_trie_tree()
{
  int ret = 0;
  if (0 != (ret = create_trie_tree(Mysql_none_reserved_keywords, LENGTH_OF(Mysql_none_reserved_keywords), &mysql_none_reserved_keywords_root))) {
    (void)printf("ERROR create trie tree failed! \n");
  } else if (0 != (ret = create_trie_tree(Mysql_sql_keywords_in_pl, LENGTH_OF(Mysql_sql_keywords_in_pl), &mysql_sql_keywords_in_pl_root))) {
    (void)printf("ERROR create trie tree failed! \n");
  } else { /*do nothing*/ }
  return ret;
}

void  __attribute__((constructor)) init_mysql_non_reserved_keywords_tree()
{
  int ret = 0;
  if (0 != (ret = create_mysql_trie_tree())) {
    (void)printf("ERROR build mysql_non_reserved_keywords tree failed=>%d", ret);
  }
}

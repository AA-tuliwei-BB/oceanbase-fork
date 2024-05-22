#ifndef OCEANBASE_SQL_OB_EXPR_GTID_H_
#define OCEANBASE_SQL_OB_EXPR_GTID_H_

#include "sql/engine/expr/ob_expr_operator.h"

using namespace oceanbase::common;

namespace oceanbase
{
namespace sql
{
class ObExprGTID : public ObFuncExprOperator
{
public:
  explicit ObExprGTID(common::ObIAllocator &alloc, ObExprOperatorType type, const char *name, int32_t param_num);
  virtual ~ObExprGTID();

private:
  DISALLOW_COPY_AND_ASSIGN(ObExprGTID);
};
class ObExprGTIDSubset : public ObExprGTID
{
public:
  explicit ObExprGTIDSubset(common::ObIAllocator &alloc);
  int cg_expr(ObExprCGCtx &expr_cg_ctx, const ObRawExpr &raw_expr, ObExpr &rt_expr) const override;
  int calc_result_type2(ObExprResType &type,
                        ObExprResType &type1,
                        ObExprResType &type2,
                        common::ObExprTypeCtx &type_ctx) const override;
  static int eval_subset(const ObExpr &expr, ObEvalCtx &ctx, ObDatum &result);

private:
  DISALLOW_COPY_AND_ASSIGN(ObExprGTIDSubset);
};

class ObExprGTIDSubtract : public ObExprGTID
{
public:
  explicit ObExprGTIDSubtract(common::ObIAllocator &alloc);
  int cg_expr(ObExprCGCtx &expr_cg_ctx, const ObRawExpr &raw_expr, ObExpr &rt_expr) const override;
  int calc_result_type2(ObExprResType &type,
                        ObExprResType &type1,
                        ObExprResType &type2,
                        common::ObExprTypeCtx &type_ctx) const override;
  static int eval_subtract(const ObExpr &expr, ObEvalCtx &ctx, ObDatum &result);

private:
  DISALLOW_COPY_AND_ASSIGN(ObExprGTIDSubtract);
};

class ObExprWaitForExecutedGTIDSet : public ObExprGTID
{
public:
  explicit ObExprWaitForExecutedGTIDSet(common::ObIAllocator &alloc);
  int cg_expr(ObExprCGCtx &expr_cg_ctx, const ObRawExpr &raw_expr, ObExpr &rt_expr) const override;
  int calc_result_typeN(ObExprResType &type,
                        ObExprResType *types,
                        int64_t param_num,
                        common::ObExprTypeCtx &type_ctx) const override;
  static int eval_wait_for_executed_gtid_set(const ObExpr &expr, ObEvalCtx &ctx, ObDatum &result);

private:
  DISALLOW_COPY_AND_ASSIGN(ObExprWaitForExecutedGTIDSet);
};

class ObExprWaitUntilSQLThreadAfterGTIDs : public ObExprGTID
{
public:
  explicit ObExprWaitUntilSQLThreadAfterGTIDs(common::ObIAllocator &alloc);
  int cg_expr(ObExprCGCtx &expr_cg_ctx, const ObRawExpr &raw_expr, ObExpr &rt_expr) const override;
  int calc_result_typeN(ObExprResType &type,
                        ObExprResType *types,
                        int64_t param_num,
                        common::ObExprTypeCtx &type_ctx) const override;
  static int eval_wait_until_sql_thread_after_gtids(const ObExpr &expr, ObEvalCtx &ctx, ObDatum &result);

private:
  DISALLOW_COPY_AND_ASSIGN(ObExprWaitUntilSQLThreadAfterGTIDs);
};
}  // namespace sql
}  // namespace oceanbase
#endif  // OCEANBASE_SQL_OB_EXPR_JSON_REMOVE_H_

/**
 * Copyright (c) 2023 OceanBase
 * OceanBase CE is licensed under Mulan PubL v2.
 * You can use this software according to the terms and conditions of the Mulan PubL v2.
 * You may obtain a copy of Mulan PubL v2 at:
 *          http://license.coscl.org.cn/MulanPubL-2.0
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
 * EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
 * MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
 * See the Mulan PubL v2 for more details.
 */

#include "lib/vector/ob_vector_ip_distance.h"
namespace oceanbase
{
namespace common
{
  int ObVectorIpDistance::ip_distance_func(const float *a, const float *b, const int64_t len, double &distance) {
    int ret = OB_SUCCESS;
#if OB_USE_MULTITARGET_CODE
    if (common::is_arch_supported(ObTargetArch::AVX512)) {
      ret = common::specific::avx512::ip_distance(a, b, len, distance);
    } else if (common::is_arch_supported(ObTargetArch::AVX2)) {
      ret = common::specific::avx2::ip_distance(a, b, len, distance);
    } else {
      ret = common::specific::normal::ip_distance(a, b, len, distance);
    }
#else
    ret = common::specific::normal::ip_distance(a, b, len, distance);
#endif
    return ret;
  }
}
}
// Copyright (c) 2018-present Baidu, Inc. All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "internal_functions.h"
#include "hll_common.h"
#include <cctype>
#include <cmath>
#include <algorithm>

namespace baikaldb {
static const int32_t DATE_FORMAT_LENGTH = 128;
ExprValue round(const std::vector<ExprValue>& input) {
    if (input.size() == 0 || input[0].is_null()) {
        return ExprValue::Null();
    }
    ExprValue tmp(pb::INT64);
    tmp._u.int64_val = ::round(input[0].get_numberic<double>());
    return tmp;
}

ExprValue floor(const std::vector<ExprValue>& input) {
    if (input.size() == 0 || input[0].is_null()) {
        return ExprValue::Null();
    }
    ExprValue tmp(pb::INT64);
    tmp._u.int64_val = ::floor(input[0].get_numberic<double>());
    return tmp;
}

ExprValue ceil(const std::vector<ExprValue>& input) {
    if (input.size() == 0 || input[0].is_null()) {
        return ExprValue::Null();
    }
    ExprValue tmp(pb::INT64);
    tmp._u.int64_val = ::ceil(input[0].get_numberic<double>());
    return tmp;
}

ExprValue abs(const std::vector<ExprValue>& input) {
    if (input.size() == 0 || input[0].is_null()) {
        return ExprValue::Null();
    }
    ExprValue tmp(pb::DOUBLE);
    tmp._u.double_val = ::abs(input[0].get_numberic<double>());
    return tmp;
}

ExprValue sqrt(const std::vector<ExprValue>& input) {
    if (input.size() == 0 || input[0].is_null()) {
        return ExprValue::Null();
    }
    double val = input[0].get_numberic<double>();
    if (val < 0) {
        return ExprValue::Null();
    }
    ExprValue tmp(pb::DOUBLE);
    tmp._u.double_val = std::sqrt(val);
    return tmp;
}

ExprValue mod(const std::vector<ExprValue>& input) {
    if (input.size() < 2 || input[0].is_null() || input[1].is_null()) {
        return ExprValue::Null();
    }
    double rhs = input[1].get_numberic<double>();
    if (float_equal(rhs, 0)) {
        return ExprValue::Null();
    }
    double lhs = input[0].get_numberic<double>();
    
    ExprValue tmp(pb::DOUBLE);
    tmp._u.double_val = std::fmod(lhs, rhs);
    return tmp;
}

ExprValue rand(const std::vector<ExprValue>& input) {
    ExprValue tmp(pb::DOUBLE);
    tmp._u.double_val = butil::fast_rand_double();
    return tmp;
}

ExprValue sign(const std::vector<ExprValue>& input) {
    if (input.size() < 1 || input[0].is_null()) {
        return ExprValue::Null();
    }
    ExprValue tmp(pb::INT64);
    double val = input[0].get_numberic<double>();
    tmp._u.int64_val = val > 0 ? 1 : (val < 0 ? -1 : 0);
    return tmp;
}

ExprValue sin(const std::vector<ExprValue>& input) {
    if (input.size() < 1 || input[0].is_null()) {
        return ExprValue::Null();
    }
    ExprValue tmp(pb::DOUBLE);
    tmp._u.double_val = std::sin(input[0].get_numberic<double>());
    return tmp;
}
ExprValue asin(const std::vector<ExprValue>& input) {
    if (input.size() < 1 || input[0].is_null()) {
        return ExprValue::Null();
    }
    ExprValue tmp(pb::DOUBLE);
    double  val = input[0].get_numberic<double>();
    if (val < -1 || val > 1) {
        return ExprValue::Null();
    }
    tmp._u.double_val = std::asin(val);
    return tmp;
}

ExprValue cos(const std::vector<ExprValue>& input) {
    if (input.size() < 1 || input[0].is_null()) {
        return ExprValue::Null();
    }
    ExprValue tmp(pb::DOUBLE);
    tmp._u.double_val = std::cos(input[0].get_numberic<double>());
    return tmp;
}

ExprValue acos(const std::vector<ExprValue>& input) {
    if (input.size() < 1 || input[0].is_null()) {
        return ExprValue::Null();
    }
    double  val = input[0].get_numberic<double>();
    if (val < -1 || val > 1) {
        return ExprValue::Null();
    }
    ExprValue tmp(pb::DOUBLE);
    tmp._u.double_val = std::acos(val);
    return tmp;
}

ExprValue tan(const std::vector<ExprValue>& input) {
    if (input.size() < 1 || input[0].is_null()) {
        return ExprValue::Null();
    }
    ExprValue tmp(pb::DOUBLE);
    tmp._u.double_val = std::tan(input[0].get_numberic<double>());
    return tmp;
}

ExprValue cot(const std::vector<ExprValue>& input) {
    if (input.size() < 1 || input[0].is_null()) {
        return ExprValue::Null();
    }
    ExprValue tmp(pb::DOUBLE);
    double val = input[0].get_numberic<double>();
    double sin_val = std::sin(val);
    double cos_val = std::cos(val);
    if (float_equal(sin_val, 0)) {
        return ExprValue::Null();
    }
    tmp._u.double_val = cos_val/sin_val;
    return tmp;
}

ExprValue atan(const std::vector<ExprValue>& input) {
    if (input.size() < 1 || input[0].is_null()) {
        return ExprValue::Null();
    }
    ExprValue tmp(pb::DOUBLE);
    tmp._u.double_val = std::atan(input[0].get_numberic<double>());
    return tmp;
}

ExprValue ln(const std::vector<ExprValue>& input) {
    if (input.size() < 1 || input[0].is_null()) {
        return ExprValue::Null();
    }
    double val = input[0].get_numberic<double>();
    if (val <= 0) {
        return ExprValue::Null();
    }
    ExprValue tmp(pb::DOUBLE);
    tmp._u.double_val = std::log(input[0].get_numberic<double>());
    return tmp;
}

ExprValue log(const std::vector<ExprValue>& input) {
    if (input.size() < 2 || input[0].is_null() || input[1].is_null()) {
        return ExprValue::Null();
    }
    double base = input[0].get_numberic<double>();
    double val = input[1].get_numberic<double>();
    if (base <= 0 || val <= 0 || base == 1) {
        return ExprValue::Null();
    }
    ExprValue tmp(pb::DOUBLE);
    tmp._u.double_val = std::log(val) / std::log(base);
    return tmp;
}

ExprValue pow(const std::vector<ExprValue>& input) {
    if (input.size() < 2 || input[0].is_null() || input[1].is_null()) {
        return ExprValue::Null();
    }
    double base = input[0].get_numberic<double>();
    double exp = input[1].get_numberic<double>();
    ExprValue tmp(pb::DOUBLE);
    tmp._u.double_val = std::pow(base, exp);
    return tmp;
}

ExprValue pi(const std::vector<ExprValue>& input) {
    ExprValue tmp(pb::DOUBLE);
    tmp._u.double_val = M_PI;
    return tmp;
}

ExprValue greatest(const std::vector<ExprValue>& input) {
    bool find_flag = false;
    double ret = std::numeric_limits<double>::lowest();
    for (const auto& item : input) {
        if (item.is_null()) {
            return ExprValue::Null();
        } else {
            double val = item.get_numberic<double>();
            if (!find_flag) {
                find_flag = true;
                ret = val;
            } else {
                if (val > ret) {
                    ret = val;
                }
            }
        }
    }
    if (find_flag) {
        ExprValue tmp(pb::DOUBLE);
        tmp._u.double_val = ret;
        return tmp;
    } else {
        return ExprValue::Null();
    }
}

ExprValue least(const std::vector<ExprValue>& input) {
    bool find_flag = false;
    double ret = std::numeric_limits<double>::max();
    for (const auto& item : input) {
        if (item.is_null()) {
            return ExprValue::Null();
        } else {
            double val = item.get_numberic<double>();
            if (!find_flag) {
                find_flag = true;
                ret = val;
            } else {
                if (val < ret) {
                    ret = val;
                }
            }
        }
    }
    if (find_flag) {
        ExprValue tmp(pb::DOUBLE);
        tmp._u.double_val = ret;
        return tmp;
    } else {
        return ExprValue::Null();
    }
}

ExprValue length(const std::vector<ExprValue>& input) {
    if (input.size() == 0 || input[0].is_null()) {
        return ExprValue::Null();
    }
    ExprValue tmp(pb::UINT32);
    tmp._u.uint32_val = input[0].get_string().size();
    return tmp;
}

ExprValue lower(const std::vector<ExprValue>& input) {
    if (input.size() == 0 || input[0].is_null()) {
        return ExprValue::Null();
    }
    ExprValue tmp(pb::STRING);
    tmp.str_val = input[0].get_string();
    std::transform(tmp.str_val.begin(), tmp.str_val.end(), tmp.str_val.begin(), ::tolower); 
    return tmp;
}

ExprValue lower_gbk(const std::vector<ExprValue>& input) {
    if (input.size() == 0 || input[0].is_null()) {
        return ExprValue::Null();
    }
    ExprValue tmp(pb::STRING);
    tmp.str_val = input[0].get_string();
    std::string& literal = tmp.str_val;
    size_t idx = 0;
    while (idx < literal.size()) {
        if ((literal[idx] & 0x80) != 0) {
            idx += 2;
        } else {
            literal[idx] = tolower(literal[idx]);
            idx++;
        }
    }
    return tmp;
}

ExprValue upper(const std::vector<ExprValue>& input) {
    if (input.size() == 0 || input[0].is_null()) {
        return ExprValue::Null();
    }
    ExprValue tmp(pb::STRING);
    tmp.str_val = input[0].get_string();
    std::transform(tmp.str_val.begin(), tmp.str_val.end(), tmp.str_val.begin(), ::toupper); 
    return tmp;
}

ExprValue concat(const std::vector<ExprValue>& input) {
    ExprValue tmp(pb::STRING);
    for (auto& s : input) {
        if (s.is_null()) {
            return ExprValue::Null();
        }
        tmp.str_val += s.get_string();
    }
    return tmp;
}

ExprValue substr(const std::vector<ExprValue>& input) {
    if (input.size() < 2) {
        return ExprValue::Null();
    }
    for (auto& s : input) {
        if (s.is_null()) {
            return ExprValue::Null();
        }
    }
    std::string str = input[0].get_string();
    ExprValue tmp(pb::STRING);
    int pos = input[1].get_numberic<int>();
    if (pos < 0) {
        pos = str.size() + pos;
    } else {
        --pos;
    }
    if (pos < 0 || pos >= (int)str.size()) {
        return tmp;
    }
    int len = -1;
    if (input.size() == 3) {
        len = input[2].get_numberic<int>();
        if (len <= 0) {
            return tmp;
        }
    }
    tmp.str_val = str;
    if (len == -1) {
        tmp.str_val = tmp.str_val.substr(pos);
    } else {
        tmp.str_val = tmp.str_val.substr(pos, len);
    }
    return tmp;
}

ExprValue left(const std::vector<ExprValue>& input) {
    if (input.size() < 2) {
        return ExprValue::Null();
    }
    for (auto& s : input) {
        if (s.is_null()) {
            return ExprValue::Null();
        }
    }
    ExprValue tmp(pb::STRING);
    int len = input[1].get_numberic<int>();
    if (len <= 0) {
        return tmp;
    }
    tmp.str_val = input[0].str_val;
    tmp.str_val = tmp.str_val.substr(0, len);
    return tmp;
}

ExprValue right(const std::vector<ExprValue>& input) {
    if (input.size() < 2) {
        return ExprValue::Null();
    }
    for (auto& s : input) {
        if (s.is_null()) {
            return ExprValue::Null();
        }
    }
    ExprValue tmp(pb::STRING);
    int len = input[1].get_numberic<int>();
    if (len <= 0) {
        return tmp;
    }
    int pos = input[0].str_val.size() - len;
    if (pos < 0) {
        pos = 0;
    }
    tmp.str_val = input[0].str_val;
    tmp.str_val = tmp.str_val.substr(pos);
    return tmp;
}

ExprValue unix_timestamp(const std::vector<ExprValue>& input) {
    ExprValue tmp(pb::UINT32);
    if (input.size() == 0) {
        tmp._u.uint32_val = time(NULL);
    } else {
        if (input[0].is_null()) {
            return ExprValue::Null();
        }
        tmp = input[0];
        tmp.cast_to(pb::TIMESTAMP);
        tmp.type = pb::UINT32;
    }
    return tmp;
}

ExprValue from_unixtime(const std::vector<ExprValue>& input) {
    if (input.size() == 0) {
        return ExprValue::Null();
    }
    ExprValue tmp(pb::TIMESTAMP);
    if (input[0].is_null()) {
        return ExprValue::Null();
    }
    tmp._u.uint32_val = input[0].get_numberic<uint32_t>();
    return tmp;
}

ExprValue now(const std::vector<ExprValue>& input) {
    return ExprValue::Now();
}
ExprValue date_format(const std::vector<ExprValue>& input) {
    if (input.size() != 2) {
        return ExprValue::Null();
    }
    for (auto& s : input) {
        if (s.is_null()) {
            return ExprValue::Null();
        }
    }
    ExprValue tmp = input[0];
    time_t t = tmp.cast_to(pb::TIMESTAMP)._u.uint32_val;
    struct tm t_result;
    localtime_r(&t, &t_result);
    char s[DATE_FORMAT_LENGTH];
    strftime(s, sizeof(s), input[1].str_val.c_str(), &t_result);
    ExprValue format_result(pb::STRING);
    format_result.str_val = s;
    return format_result;
}
ExprValue timediff(const std::vector<ExprValue>& input) {
    if (input.size() < 2) {
        return ExprValue::Null();
    }
    for (auto& s : input) {
        if (s.is_null()) {
            return ExprValue::Null();
        }
    }
    ExprValue arg1 = input[0];
    ExprValue arg2 = input[1];
    int32_t seconds = arg1.cast_to(pb::TIMESTAMP)._u.uint32_val - 
        arg2.cast_to(pb::TIMESTAMP)._u.uint32_val;
    ExprValue ret(pb::TIME);
    ret._u.int32_val = seconds_to_time(seconds);
    return ret;
}
ExprValue timestampdiff(const std::vector<ExprValue>& input) {
    if (input.size() < 3) {
        return ExprValue::Null();
    }
    for (auto& s : input) {
        if (s.is_null()) {
            return ExprValue::Null();
        }
    }

    ExprValue arg2 = input[1];
    ExprValue arg3 = input[2];
    int32_t seconds = arg3.cast_to(pb::TIMESTAMP)._u.uint32_val - 
        arg2.cast_to(pb::TIMESTAMP)._u.uint32_val;
    ExprValue ret(pb::INT64);
    if (input[0].str_val == "second") {
        ret._u.int64_val = seconds;
    } else if (input[0].str_val == "minute") {
        ret._u.int64_val = seconds / 60;
    } else if (input[0].str_val == "hour") {
        ret._u.int64_val = seconds / 3600;
    } else if (input[0].str_val == "day") {
        ret._u.int64_val = seconds / (24 * 3600);
    } else {
        // un-support
        return ExprValue::Null();
    }
    return ret;
}

ExprValue hll_add(const std::vector<ExprValue>& input) {
    if (input.size() == 0) {
        return ExprValue::Null();
    }
    if (!input[0].is_hll()) {
        (ExprValue&)input[0] = hll::hll_init();
    }
    for (size_t i = 1; i < input.size(); i++) {
        if (!input[i].is_null()) {
            hll::hll_add((ExprValue&)input[0], input[i].hash());
        }
    }
    return input[0];
}

ExprValue hll_init(const std::vector<ExprValue>& input) {
    if (input.size() == 0) {
        return ExprValue::Null();
    }
    ExprValue hll_init = hll::hll_init();
    for (size_t i = 0; i < input.size(); i++) {
        if (!input[i].is_null()) {
            hll::hll_add(hll_init, input[i].hash());
        }
    }
    return hll_init;
}

ExprValue hll_merge(const std::vector<ExprValue>& input) {
    if (input.size() == 0) {
        return ExprValue::Null();
    }
    if (!input[0].is_hll()) {
        (ExprValue&)input[0] = hll::hll_init();
    }
    for (size_t i = 1; i < input.size(); i++) {
        if (input[i].is_hll()) {
            hll::hll_merge((ExprValue&)input[0], (ExprValue&)input[i]);
        }
    }
    return input[0];
}
ExprValue hll_estimate(const std::vector<ExprValue>& input) {
    if (input.size() == 0) {
        return ExprValue::Null();
    }
    ExprValue tmp(pb::INT64);
    if (input[0].is_null()) {
        tmp._u.int64_val = 0;
        return tmp;
    }
    tmp._u.int64_val = hll::hll_estimate(input[0]);
    return tmp;
}

ExprValue case_when(const std::vector<ExprValue>& input) {
    for (size_t i = 0; i < input.size() / 2; ++i) {
        auto if_index = i * 2;
        auto then_index = i * 2 + 1;
        if (input[if_index].get_numberic<bool>()) {
            return input[then_index];
        }
    }
    //没有else分支, 返回null
    if (input.size() % 2 == 0) {
        return ExprValue();
     } else {
        return input[input.size() - 1];
    }
}

ExprValue case_expr_when(const std::vector<ExprValue>& input) {
    for (size_t i = 0; i < (input.size() - 1) / 2; ++i) {
        auto if_index = i * 2 + 1;
        auto then_index = i * 2 + 2;
        if (input[0].compare(input[if_index]) == 0) {
            return input[then_index];
        }
    }
    //没有else分支, 返回null
    if ((input.size() - 1) % 2 == 0) {
        return ExprValue();
     } else {
        return input[input.size() - 1];
    }
}

ExprValue if_(const std::vector<ExprValue>& input) {
    if (input.size() != 3) {
        return ExprValue::Null();
    }
    return input[0].get_numberic<bool>() ? input[1] : input[2];
}

ExprValue murmur_hash(const std::vector<ExprValue>& input) {
    if (input.size() == 0) {
        return ExprValue::Null();
    }
    ExprValue tmp(pb::UINT64);
    if (input.size() == 0) {
        tmp._u.uint64_val = 0;
    } else {
        tmp._u.uint64_val = make_sign(input[0].str_val);
    }
    return tmp;
}
}

/* vim: set ts=4 sw=4 sts=4 tw=100 */

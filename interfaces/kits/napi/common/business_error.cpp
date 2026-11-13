/*
 * Copyright (c) 2022 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "business_error.h"

#include "web_errors.h"

namespace OHOS {
namespace NWebError {
void BusinessError::ThrowError(napi_env env, int32_t err, const std::string& msg)
{
    // napi_throw_error's optional code parameter is a string. BusinessError
    // exposes a numeric code instead; retain a real Error (including stack).
    napi_value error = nullptr;
    napi_value message = nullptr;
    napi_value code = nullptr;
    napi_value name = nullptr;
    if (napi_create_string_utf8(env, msg.c_str(), NAPI_AUTO_LENGTH, &message) != napi_ok ||
        napi_create_error(env, nullptr, message, &error) != napi_ok ||
        napi_create_int32(env, err, &code) != napi_ok ||
        napi_set_named_property(env, error, "code", code) != napi_ok ||
        napi_create_string_utf8(env, "BusinessError", NAPI_AUTO_LENGTH, &name) != napi_ok ||
        napi_set_named_property(env, error, "name", name) != napi_ok) {
        return;
    }
    napi_throw(env, error);
}

napi_value BusinessError::CreateError(napi_env env, int32_t err)
{
    return CreateError(env, err, GetErrMsgByErrCode(err));
}

napi_value BusinessError::CreateError(napi_env env, int32_t err, const std::string& errorMsg)
{
    napi_value businessError = nullptr;
    NAPI_CALL(env, napi_create_object(env, &businessError));
    napi_value errorCode = nullptr;
    NAPI_CALL(env, napi_create_int32(env, err, &errorCode));
    napi_value errorMessage = nullptr;
    NAPI_CALL(env, napi_create_string_utf8(env, errorMsg.c_str(), NAPI_AUTO_LENGTH, &errorMessage));
    NAPI_CALL(env, napi_set_named_property(env, businessError, "code", errorCode));
    NAPI_CALL(env, napi_set_named_property(env, businessError, "message", errorMessage));
    return businessError;
}

void BusinessError::ThrowErrorByErrcode(napi_env env, int32_t errCode)
{
    ThrowError(env, errCode, GetErrMsgByErrCode(errCode));
}

void BusinessError::ThrowErrorByErrcode(napi_env env, int32_t errCode, const std::string& errorMsg)
{
    ThrowError(env, errCode, errorMsg);
}
}
}

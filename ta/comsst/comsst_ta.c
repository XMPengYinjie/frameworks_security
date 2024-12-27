/*
 * Copyright (C) 2022-2024 Xiaomi Corporation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <comsst_ta.h>
#include <kernel/user_ta.h>
#include <string.h>
#include <tee_internal_api.h>
#include <trace.h>

static TEE_Result Comsst_CheckItem(uint32_t param_types __unused,
    TEE_Param params[4] __unused);
static TEE_Result Comsst_DeleteItem(uint32_t param_types __unused,
    TEE_Param params[4] __unused);
static TEE_Result Comsst_ReadItem(uint32_t param_types __unused,
    TEE_Param params[4] __unused);
static TEE_Result Comsst_WriteItem(uint32_t param_types __unused,
    TEE_Param params[4] __unused);
static TEE_Result Comsst_VerifyItem(uint32_t param_types __unused,
    TEE_Param params[4] __unused);

/*
 * Called when the instance of the TA is created. This is the first call in
 * the TA.
 */
TEE_Result COMSST_TA_CreateEntryPoint(void)
{
    DMSG("has been called\n");

    return TEE_SUCCESS;
}

/*
 * Called when the instance of the TA is destroyed if the TA has not
 * crashed or panicked. This is the last call in the TA.
 */
void COMSST_TA_DestroyEntryPoint(void)
{
    DMSG("has been called\n");
}

/*
 * Called when a new session is opened to the TA. *sess_ctx can be updated
 * with a value to be able to identify this session in subsequent calls to the
 * TA. In this function you will normally do the global initialization for the
 * TA.
 */
TEE_Result COMSST_TA_OpenSessionEntryPoint(uint32_t param_types,
    TEE_Param __maybe_unused params[4],
    void __maybe_unused** sess_ctx)
{
    uint32_t exp_param_types = TEE_PARAM_TYPES(TEE_PARAM_TYPE_VALUE_INPUT,
        TEE_PARAM_TYPE_NONE,
        TEE_PARAM_TYPE_NONE,
        TEE_PARAM_TYPE_NONE);

    DMSG("has been called\n");
    if (param_types != exp_param_types) {
        EMSG("718fc92c\n");
        return TEE_ERROR_BAD_PARAMETERS;
    }
    /*
     * The DMSG() macro is non-standard, TEE Internal API doesn't
     * specify any means to logging from a TA.
     */
    DMSG("COMSST TA!\n");

    /* If return value != TEE_SUCCESS the session will not be created. */
    return TEE_SUCCESS;
}

/*
 * Called when a session is closed, sess_ctx hold the value that was
 * assigned by TA_OpenSessionEntryPoint().
 */
void COMSST_TA_CloseSessionEntryPoint(void __maybe_unused* sess_ctx)
{
    (void)&sess_ctx; /* Unused parameter */
    DMSG("Goodbye!\n");
}

/*
 * Called when a TA is invoked. sess_ctx hold that value that was
 * assigned by TA_OpenSessionEntryPoint(). The rest of the paramters
 * comes from normal world.
 */
TEE_Result COMSST_TA_InvokeCommandEntryPoint(void __maybe_unused* sess_ctx,
    uint32_t cmd_id,
    uint32_t param_types, TEE_Param params[4])
{
    (void)&sess_ctx; /* Unused parameter */

    DMSG("cmd: 0x%08" PRIx32 "\n", cmd_id);
    switch (cmd_id) {
    case TA_COMSST_CMD_CHK:
        return Comsst_CheckItem(param_types, params);
    case TA_COMSST_CMD_DEL:
        return Comsst_DeleteItem(param_types, params);
    case TA_COMSST_CMD_WR:
        return Comsst_WriteItem(param_types, params);
    case TA_COMSST_CMD_RD:
        return Comsst_ReadItem(param_types, params);
    case TA_COMSST_CMD_VR:
        return Comsst_VerifyItem(param_types, params);
    default:
        EMSG("ee962c07:0x%08" PRIx32 "\n", cmd_id);
        return TEE_ERROR_BAD_PARAMETERS;
    }
}

static TEE_Result Comsst_CheckItem(uint32_t param_types __unused,
    TEE_Param params[4] __unused)
{
    TEE_Result res = TEE_ERROR_GENERIC;
    TEE_ObjectHandle obj;
    uint32_t exp_param_types = TEE_PARAM_TYPES(TEE_PARAM_TYPE_VALUE_INPUT,
        TEE_PARAM_TYPE_MEMREF_INPUT,
        TEE_PARAM_TYPE_NONE,
        TEE_PARAM_TYPE_NONE);

    if (param_types != exp_param_types) {
        EMSG("718fc92c\n");
        return TEE_ERROR_BAD_PARAMETERS;
    }

    DMSG("TEE_OpenPersistentObject()...\n");

    if (params[0].value.b == 0) {
        res = TEE_OpenPersistentObject(TEE_STORAGE_PRIVATE,
            params[1].memref.buffer,
            params[0].value.a,
            TEE_DATA_FLAG_ACCESS_READ,
            &obj);
    } else {
        res = TEE_OpenPersistentObject(TEE_STORAGE_USER,
            params[1].memref.buffer,
            params[0].value.a,
            TEE_DATA_FLAG_ACCESS_READ,
            &obj);
    }

    if (res != TEE_SUCCESS) {
        EMSG("c173d631:0x%08" PRIx32 "\n", res);
        goto exit;
    }

    DMSG("TEE_CloseObject()...\n");
    TEE_CloseObject(obj);

exit:
    return res;
}

static TEE_Result Comsst_DeleteItem(uint32_t param_types __unused,
    TEE_Param params[4] __unused)
{
    TEE_Result res = TEE_ERROR_GENERIC;
    TEE_ObjectHandle obj;
    uint32_t exp_param_types = TEE_PARAM_TYPES(TEE_PARAM_TYPE_VALUE_INPUT,
        TEE_PARAM_TYPE_MEMREF_INPUT,
        TEE_PARAM_TYPE_NONE,
        TEE_PARAM_TYPE_NONE);

    if (param_types != exp_param_types) {
        EMSG("718fc92c\n");
        return TEE_ERROR_BAD_PARAMETERS;
    }

    DMSG("TEE_OpenPersistentObject()...\n");

    if (params[0].value.b == 0) {
        res = TEE_OpenPersistentObject(TEE_STORAGE_PRIVATE,
            params[1].memref.buffer,
            params[0].value.a,
            TEE_DATA_FLAG_ACCESS_WRITE_META,
            &obj);
    } else {
        res = TEE_OpenPersistentObject(TEE_STORAGE_USER,
            params[1].memref.buffer,
            params[0].value.a,
            TEE_DATA_FLAG_ACCESS_WRITE_META,
            &obj);
    }

    if (res != TEE_SUCCESS) {
        EMSG("c173d631:0x%08" PRIx32 "\n", res);
        return res;
    }

    DMSG("TEE_CloseAndDeletePersistentObject1()...\n");
    res = TEE_CloseAndDeletePersistentObject1(obj);
    if (res != TEE_SUCCESS) {
        EMSG("69aa1fce:0x%08" PRIx32 "\n", res);
        return res;
    }
    return TEE_SUCCESS;
}

static TEE_Result Comsst_ReadItem(uint32_t param_types __unused,
    TEE_Param params[4] __unused)
{
    TEE_Result res = TEE_ERROR_GENERIC;
    TEE_ObjectHandle obj;
    size_t read_len;
    uint32_t exp_param_types = TEE_PARAM_TYPES(TEE_PARAM_TYPE_VALUE_INOUT,
        TEE_PARAM_TYPE_MEMREF_INOUT,
        TEE_PARAM_TYPE_NONE,
        TEE_PARAM_TYPE_NONE);

    if (param_types != exp_param_types) {
        EMSG("718fc92c\n");
        return TEE_ERROR_BAD_PARAMETERS;
    }

    DMSG("TEE_OpenPersistentObject()...\n");

    if (params[0].value.b == 0) {
        res = TEE_OpenPersistentObject(TEE_STORAGE_PRIVATE,
            params[1].memref.buffer,
            params[0].value.a,
            TEE_DATA_FLAG_ACCESS_READ,
            &obj);
    } else {
        res = TEE_OpenPersistentObject(TEE_STORAGE_USER,
            params[1].memref.buffer,
            params[0].value.a,
            TEE_DATA_FLAG_ACCESS_READ,
            &obj);
    }

    if (res != TEE_SUCCESS) {
        EMSG("c173d631:0x%08" PRIx32 "\n", res);
        return res;
    }

    DMSG("TEE_ReadObjectData()...\n");

    res = TEE_ReadObjectData(obj, params[1].memref.buffer,
        params[1].memref.size, &read_len);
    if (res != TEE_SUCCESS) {
        EMSG("8d4785a7:0x%08" PRIx32 ",%zu\n",
            res, read_len);
        goto exit;
    }

    params[0].value.b = read_len;

exit:
    DMSG("TEE_CloseObject()...\n");
    TEE_CloseObject(obj);
    return res;
}

static TEE_Result Comsst_WriteItem(uint32_t param_types __unused,
    TEE_Param params[4] __unused)
{
    TEE_Result res = TEE_ERROR_GENERIC;
    TEE_ObjectHandle obj;

    uint32_t exp_param_types = TEE_PARAM_TYPES(TEE_PARAM_TYPE_VALUE_INPUT,
        TEE_PARAM_TYPE_MEMREF_INPUT,
        TEE_PARAM_TYPE_NONE,
        TEE_PARAM_TYPE_NONE);

    if (param_types != exp_param_types) {
        EMSG("718fc92c\n");
        return TEE_ERROR_BAD_PARAMETERS;
    }

    DMSG("TEE_CreatePersistentObject...\n");

    if (params[0].value.b == 0) {
        res = TEE_CreatePersistentObject(TEE_STORAGE_PRIVATE,
            params[1].memref.buffer, params[0].value.a,
            TEE_DATA_FLAG_ACCESS_WRITE_META | TEE_DATA_FLAG_ACCESS_READ | TEE_DATA_FLAG_ACCESS_WRITE | TEE_DATA_FLAG_OVERWRITE, NULL,
            NULL, 0, &obj);
    } else {
        res = TEE_CreatePersistentObject(TEE_STORAGE_USER,
            params[1].memref.buffer, params[0].value.a,
            TEE_DATA_FLAG_ACCESS_WRITE_META | TEE_DATA_FLAG_ACCESS_READ | TEE_DATA_FLAG_ACCESS_WRITE | TEE_DATA_FLAG_OVERWRITE, NULL,
            NULL, 0, &obj);
    }

    if (res != TEE_SUCCESS) {
        EMSG("e23a89fe:0x%08" PRIx32 "\n", res);
        return res;
    }

    DMSG("TEE_WriteObjectData()...\n");

    res = TEE_WriteObjectData(obj, params[1].memref.buffer + params[0].value.a,
        params[1].memref.size - params[0].value.a);

    DMSG("TEE_CloseObject()...\n");
    TEE_CloseObject(obj);
    return res;
}

static TEE_Result Comsst_VerifyItem(uint32_t param_types __unused,
    TEE_Param params[4] __unused)
{
    TEE_Result res = TEE_ERROR_GENERIC;
    TEE_ObjectHandle obj;

    uint32_t exp_param_types = TEE_PARAM_TYPES(TEE_PARAM_TYPE_VALUE_INPUT,
        TEE_PARAM_TYPE_MEMREF_INPUT,
        TEE_PARAM_TYPE_NONE,
        TEE_PARAM_TYPE_NONE);

    size_t read_len;
    static uint8_t data[512];

    if (param_types != exp_param_types || params[0].value.a > sizeof(data)) {
        EMSG("718fc92c\n");
        return TEE_ERROR_BAD_PARAMETERS;
    }

    DMSG("TEE_OpenPersistentObject...\n");

    if (params[0].value.b == 0) {
        res = TEE_OpenPersistentObject(TEE_STORAGE_PRIVATE,
            params[1].memref.buffer,
            params[0].value.a,
            TEE_DATA_FLAG_ACCESS_READ,
            &obj);
    } else {
        res = TEE_OpenPersistentObject(TEE_STORAGE_USER,
            params[1].memref.buffer,
            params[0].value.a,
            TEE_DATA_FLAG_ACCESS_READ,
            &obj);
    }

    if (res != TEE_SUCCESS) {
        EMSG("c173d631:0x%08" PRIx32 "\n", res);
        return res;
    }

    DMSG("TEE_ReadObjectData()...\n");

    res = TEE_ReadObjectData(obj, data, sizeof(data), &read_len);
    if (res != TEE_SUCCESS) {
        EMSG("8d4785a7:0x%08" PRIx32 ",%zu\n",
            res, read_len);
        goto exit;
    }

    if (read_len != params[1].memref.size - params[0].value.a) {
        res = TEE_ERROR_GENERIC;
        goto exit;
    }

    for (int i = 0; i < read_len; i++) {
        if (data[i] != ((uint8_t*)params[1].memref.buffer)[i + params[0].value.a]) {
            res = TEE_ERROR_GENERIC;
            goto exit;
        }
    }

exit:
    DMSG("TEE_CloseObject()...\n");
    TEE_CloseObject(obj);
    return res;
}

struct user_ta_head comsst_user_ta_head = {
    .uuid = TA_COMSST_UUID,
    .name = "COMSST",
    .flags = TA_FLAG_USER_MODE,
    .create_entry_point = COMSST_TA_CreateEntryPoint,
    .destroy_entry_point = COMSST_TA_DestroyEntryPoint,
    .open_session_entry_point = COMSST_TA_OpenSessionEntryPoint,
    .close_session_entry_point = COMSST_TA_CloseSessionEntryPoint,
    .invoke_command_entry_point = COMSST_TA_InvokeCommandEntryPoint
};

struct user_ta_head* user_ta = &comsst_user_ta_head;
/*******************************************************************************
*   (c) 2018 Zondax GmbH
*
*  Licensed under the Apache License, Version 2.0 (the "License");
*  you may not use this file except in compliance with the License.
*  You may obtain a copy of the License at
*
*      http://www.apache.org/licenses/LICENSE-2.0
*
*  Unless required by applicable law or agreed to in writing, software
*  distributed under the License is distributed on an "AS IS" BASIS,
*  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*  See the License for the specific language governing permissions and
*  limitations under the License.
********************************************************************************/
#pragma once

#include "zxmacros.h"
#include <stdbool.h>
#include "zxerror.h"
#include "coin.h"
//fixme: maybe increase some of these
#define T_IN_LIST_SIZE                 5
#define T_OUT_LIST_SIZE                 5
#define SPEND_LIST_SIZE                 5
#define OUTPUT_LIST_SIZE                 5

typedef struct {
    uint32_t path[5];
    uint8_t script[26];
    uint64_t value;
} t_input_item_t;

typedef struct {
    uint8_t address[26];
    uint64_t value;
} t_output_item_t;

typedef struct {
    t_input_item_t items[T_IN_LIST_SIZE];
} t_inlist_t;

typedef struct {
    t_output_item_t items[T_OUT_LIST_SIZE];
} t_outlist_t;

// TODO: pragma packing 1
typedef struct {
    uint32_t path;
    uint64_t value;
    uint8_t div[11];
    uint8_t pkd[32];
    uint8_t rcm[32];
    uint8_t alpha[32];
} spend_item_t;

typedef struct {
    uint64_t total_value;
    uint8_t state;
    uint8_t session_key_set;
    uint8_t t_in_len;
    uint8_t t_out_len;
    uint8_t spendlist_len;
    uint8_t outputlist_len;
    uint8_t spendlist_extract_index;
    uint8_t outputlist_extract_index;
    uint8_t spends_sign_index;
    uint8_t spends_sign_extract_index;
    uint8_t t_sign_index;
    uint8_t t_sign_extract_index;
} transaction_header_t;

typedef struct {
    spend_item_t items[SPEND_LIST_SIZE];
} spendlist_t;

typedef struct {
    uint8_t div[11];
    uint8_t pkd[32];
    uint64_t value;
    uint8_t memotype;
    uint8_t rcmvalue[32];
    uint8_t rseed[32];
    uint8_t ovk[32];
} output_item_t;

typedef struct {
    output_item_t items[SPEND_LIST_SIZE];
} outputlist_t;

typedef struct {
    uint8_t session_key[32];
    uint8_t transparent_signatures[T_IN_LIST_SIZE][64];
    uint8_t spend_signatures[SPEND_LIST_SIZE][64];
} transaction_info_t;

#ifdef __cplusplus
extern "C" {
#endif

void transaction_reset();

void set_session_key(uint8_t *key);

uint8_t *get_session_key();

//statemachine API
uint8_t get_state();

void state_reset();

void set_state(uint8_t state);

#define STATE_INITIAL                           0
#define STATE_PROCESSED_INPUTS                  1
#define STATE_PROCESSED_SPEND_EXTRACTIONS       2
#define STATE_PROCESSED_ALL_EXTRACTIONS         3
#define STATE_VERIFIED_ALL_TXDATA               4

//metadata flash api
uint64_t get_valuebalance();

bool spendlist_first_sign();

bool spendlist_more_sign();

spend_item_t *spendlist_sign_next();

bool transparent_signatures_more_extract();

void transparent_signatures_append(uint8_t *signature);

zxerr_t get_next_transparent_signature(uint8_t *result);

bool spend_signatures_more_extract();

void spend_signatures_append(uint8_t *signature);

zxerr_t get_next_spend_signature(uint8_t *result);
//transparent TxIN API
zxerr_t t_inlist_append_item(uint32_t *p, uint8_t *script, uint64_t v);

t_input_item_t *t_inlist_retrieve_item(uint8_t i);

uint8_t t_inlist_len();


//transparent TXOUT API
zxerr_t t_outlist_append_item(uint8_t *addr, uint64_t v);

t_output_item_t *t_outlist_retrieve_item(uint8_t i);

uint8_t t_outlist_len();

//spendlist flashstorage API
bool spendlist_is_active();

zxerr_t spendlist_append_item(uint32_t p, uint64_t v, uint8_t *div, uint8_t *pkd, uint8_t *rcm, uint8_t *alpha);

uint8_t spendlist_len();

void spendlist_reset();

spend_item_t *spendlist_retrieve_item(uint8_t i);

spend_item_t *spendlist_extract_next();

bool spendlist_more_extract();

//outputlist flashstorage API
bool outputlist_is_active();

zxerr_t outputlist_append_item(uint8_t *d, uint8_t *pkd, uint64_t v, uint8_t memotype, uint8_t *ovk, uint8_t *rcmv,
                               uint8_t *rseed);

uint8_t outputlist_len();

void outputlist_reset();

output_item_t *outputlist_retrieve_item(uint8_t i);

output_item_t *outputlist_extract_next();

bool outputlist_more_extract();

void zeroize_flashstorage();

#ifdef __cplusplus
}
#endif
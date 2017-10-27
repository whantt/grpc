/*
 *
 * Copyright 2016 gRPC authors.
 *
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
 *
 */

#include <grpc/support/alloc.h>
#include <grpc/support/log.h>

#include "src/core/lib/iomgr/polling_entity.h"

grpc_polling_entity grpc_polling_entity_create_from_pollset_set(
    grpc_pollset_set *pollset_set) {
  grpc_polling_entity pollent;
  pollent.pollent.pollset_set = pollset_set;
  pollent.tag = POPS_POLLSET_SET;
  return pollent;
}

grpc_polling_entity grpc_polling_entity_create_from_pollset(
    grpc_pollset *pollset) {
  grpc_polling_entity pollent;
  pollent.pollent.pollset = pollset;
  pollent.tag = POPS_POLLSET;
  return pollent;
}

grpc_pollset *grpc_polling_entity_pollset(grpc_polling_entity *pollent) {
  if (pollent->tag == POPS_POLLSET) {
    return pollent->pollent.pollset;
  }
  return NULL;
}

grpc_pollset_set *grpc_polling_entity_pollset_set(
    grpc_polling_entity *pollent) {
  if (pollent->tag == POPS_POLLSET_SET) {
    return pollent->pollent.pollset_set;
  }
  return NULL;
}

bool grpc_polling_entity_is_empty(const grpc_polling_entity *pollent) {
  return pollent->tag == POPS_NONE;
}

void grpc_polling_entity_add_to_pollset_set(grpc_exec_ctx *exec_ctx,
                                            grpc_polling_entity *pollent,
                                            grpc_pollset_set *pss_dst) {
  if (pollent->tag == POPS_POLLSET) {
    GPR_ASSERT(pollent->pollent.pollset != NULL);
    grpc_pollset_set_add_pollset(exec_ctx, pss_dst, pollent->pollent.pollset);
  } else if (pollent->tag == POPS_POLLSET_SET) {
    GPR_ASSERT(pollent->pollent.pollset_set != NULL);
    grpc_pollset_set_add_pollset_set(exec_ctx, pss_dst,
                                     pollent->pollent.pollset_set);
  } else {
    gpr_log(GPR_ERROR, "Invalid grpc_polling_entity tag '%d'", pollent->tag);
    abort();
  }
}

void grpc_polling_entity_del_from_pollset_set(grpc_exec_ctx *exec_ctx,
                                              grpc_polling_entity *pollent,
                                              grpc_pollset_set *pss_dst) {
  if (pollent->tag == POPS_POLLSET) {
    GPR_ASSERT(pollent->pollent.pollset != NULL);
    grpc_pollset_set_del_pollset(exec_ctx, pss_dst, pollent->pollent.pollset);
  } else if (pollent->tag == POPS_POLLSET_SET) {
    GPR_ASSERT(pollent->pollent.pollset_set != NULL);
    grpc_pollset_set_del_pollset_set(exec_ctx, pss_dst,
                                     pollent->pollent.pollset_set);
  } else {
    gpr_log(GPR_ERROR, "Invalid grpc_polling_entity tag '%d'", pollent->tag);
    abort();
  }
}
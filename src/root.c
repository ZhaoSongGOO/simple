#include "root.h"
#include "log.h"
#include "system.h"
#include <stdint.h>

void C_SVC_Handler(uint32_t *svc_args) {
  uint32_t service_id = svc_args[0];
  switch (service_id) {
  case SYS_PRINT:
    Trigger_SVC_Print();
    break;
  default:
    Log.w("Unknown id!");
    break;
  }
}
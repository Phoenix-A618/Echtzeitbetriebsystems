#include "Busdata.h"
#include <string.h>

void sdata_init(sbusdata *data) {
    memset(data, 0, sizeof(sbusdata));
}

void adata_init(abusdata *data) {
    memset(data, 0, sizeof(abusdata));
}

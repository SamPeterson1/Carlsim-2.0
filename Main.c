#include <stdio.h>
#include "Movegen.h"
#include "Move.h"
#include "StrUtil.h"
#include "Log.h"
#include "Fen.h"
#include "io.h"
#include "Perft.h"
#include "uci.h"

int main(void) {
    char cmd[1024];
    
    setbuf(stdout, NULL);
    
    //Execute command read from stdin
    while (1) {
        fgets(cmd, 1024, stdin);

        if (uci_exec(cmd)) 
            return 0;
    }
}
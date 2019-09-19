;
; STARTER.ASM
;
; (c) Copyright 2002, P. Jakubco ml.
;
;
;

                .model  compact
                .stack  16384
                .386p
                .code
                extrn   _main: near

                .startup
                call    _main
                xor     ax,ax
                .exit

                end

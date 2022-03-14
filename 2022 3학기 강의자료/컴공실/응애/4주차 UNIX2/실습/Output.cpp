#include "Header.h"

void Print_Line( char *line, int *Count, int *B_Flag ) {
	int i, N_Blanks, N_Chars, Start=0; /// 초기화 되지 않은 변수 (Start) 초기화

	Get_Blanks_Chars( line, Start, &N_Blanks, &N_Chars );
	while ( N_Chars != 0 ) {
		if ( (*B_Flag == 1) && ((*Count + N_Chars + 1) <= LIMIT) ) {

			/* In this part, N_Blanks == 0 in order that L1 and L2 should be merged.
			* "N_Blanks != 0" means that L2 is written at the next line.
			*/
			if ( N_Blanks != 0 ) {
				printf("Something Wrong!\n");
				exit(-1);
			}

			putchar(' ');
			*B_Flag = 0;
			for ( i = Start; i < Start + N_Chars; i++ ) { /// 기존:i < Start + N_Chars - 1
				putchar(line[i]);
			}
			Start = Start + N_Chars;
			*Count = *Count + N_Chars + 1;
			Get_Blanks_Chars( line, Start, &N_Blanks, &N_Chars );
		}
		else if ( (*Count + N_Blanks + N_Chars) <= LIMIT ) {
            for ( i = Start; i < Start + N_Blanks + N_Chars; i++ ) { /// 기존:i < Start + N_Chars - 1
                putchar(line[i]);
            }
            Start = Start + N_Blanks + N_Chars;
            *Count = *Count + N_Blanks + N_Chars;               /// Count 갱신
            Get_Blanks_Chars( line, Start, &N_Blanks, &N_Chars );
		}
		/// 이상한 구조의 if-else if-else 문 구조 변경
        else if ( *Count == 0 ) {
            for ( i = Start; i < Start + N_Blanks + N_Chars; i++ ) {
                putchar(line[i]);
            }
            Start = Start + N_Blanks + N_Chars;
            putchar('\n');
            Get_Blanks_Chars( line, Start, &N_Blanks, &N_Chars );
            Start = Start + N_Blanks;
        }
        else {
            putchar('\n');
            *B_Flag = 0;
            for ( i = Start + N_Blanks; i < Start + N_Blanks + N_Chars; i++ ) { /// 기존:i < Start + N_Chars - 1
                putchar(line[i]);
            }
            Start = Start + N_Blanks + N_Chars;
            *Count = N_Chars;
            Get_Blanks_Chars( line, Start, &N_Blanks, &N_Chars );
        }
    }
}

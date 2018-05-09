#include <stdio.h>

void print_grade(int grade) {
	if (grade == 100) {
		printf(" __________________________________________________ \n");
		printf("| Windows Dialog                      [-] [ ] [X]  |\n");
		printf("| ''''''''''''''                                   |\n");
		printf("| Windows has detected that YOU are awesome!       |\n");
		printf("|    Do you agree?                                 |\n");
		printf("|                                                  |\n");
		printf("|         |YES!|      |YES!|      |YES!|           |\n");
		printf("|                                                  |\n");
		printf("|__________________________________________________|\n");
		printf("               Now go grab a beer!                  \n");
	}
	if (grade <= 99 && grade > 55) {
		printf("\n 		Not that much left!					             \n");
		printf("                                      &&   &&            \n");
		printf("  &&&&.                         &&&  .&&&&&              \n");
		printf("      &&     &&&                   &&&&&&&&&             \n");
		printf(" &&&&&&&&&  &         &`           &&       &&           \n");
		printf("      .&&&&&          &&&;        &8                     \n");
		printf("  .&&&:     &&        &` &      &&                       \n");
		printf("             8&&      &   `&   &&                        \n");
		printf("               &&    .&_ oO_&.-.-.                       \n");
		printf("                 && ( __     -/--'                       \n");
		printf("                  &&~ .'-__-'&                           \n");
		printf("                   &&&~`'\\`&                            \n");
		printf("                     &&&~` _&                            \n");
		printf("                      &&&&` &&                           \n");
		printf("                 &     &&8&&&&                           \n");
		printf("                  &    &&&&&&&                           \n");
		printf("                   &  &&&&&&&                            \n");
		printf("                 &&;&&&&&&&                              \n");
		printf("                &&&&&&&&&&     ~~~                       \n");
		printf("         .~~~~~ `&&&&&&&&&    ~~~~                       \n");
		printf("     ~~~~~~~~~;!&&&&&&&&&&~/~~~~~~~~~                    \n");
		printf("  ~~~~~~~~\\~~~~&/`      \\`~~~~~~~~~~~~                 \n");
		printf("   ~~~~~~~~             `~~~~~~~~~~                      \n");
		printf("    ~~~~~~              ~~~~~~~                          \n");
		printf("    ~~~~                ~~~~                             \n");
	}
	if (grade <= 55)
		printf(" \t  ___________\n\t/   R.I.P.   \\ \n\t| Here lies   |\n\t| Your  grade |\n\t|_____%2d______|\n", grade);
}

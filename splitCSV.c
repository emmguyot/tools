/* 
 * This file is part of the emmguyot tools distribution (https://github.com/emmguyot/tools).
 * Copyright (c) 2023 Emmanuel Guyot.
 * 
 * This program is free software: you can redistribute it and/or modify  
 * it under the terms of the GNU General Public License as published by  
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but 
 * WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License 
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int main (int argc, char** argv) {

    char* input = argv[1];
    FILE* f = fopen(input, "rb");
    FILE* fout = NULL;
    int numFich = 1;

    if (f == NULL) {
        printf ("Fichier inconnu\n");
        return -1;
    }

    int numLigne = 0;
    char entete[2000];
    int tailleLigne = 0;
    int dansChamp = false;
    int previous = '\n';
    do {
        int c = fgetc(f);

        if (!feof(f)) {

            if (fout == NULL) {
                char nomfichier[100];
                sprintf(nomfichier, "split-%.3i.csv", numFich++);
                fout = fopen(nomfichier, "wb");
                if (fout == NULL) {
                    printf("Impossible d'écrire %s\n", nomfichier);
                    fclose (f);
                    return -2;
                }
            }
            if (c != '\n') {
                if (c == '"') {
                    if (dansChamp == false) {
                        dansChamp = true;
                    }
                    else {
                        if (previous == '"') {
                            // Ignore la double quote
                        }
                        else {
                            dansChamp = false;
                        }
                    }
                }

                fputc(c, fout);
                if (numLigne == 0) {
                    // Stocke l'entête
                    entete[tailleLigne++] = c;
                }
            }
            else
            {
                if (dansChamp) {
                    fputc(c, fout);
                }
                else {
                    fputc(c, fout);

                    if (numLigne == 0) { 
                        entete[tailleLigne++] = '\n';
                        entete[tailleLigne++] = 0;
                    }
                    numLigne++;

                    if (numLigne % 10000 == 0) {
                        fclose(fout);
                        char nomfichier[100];
                        sprintf(nomfichier, "split-%.3i.csv", numFich++);
                        fout = fopen(nomfichier, "wb");
                        if (fout == NULL) {
                            printf("Impossible d'écrire %s\n", nomfichier);
                            fclose (f);
                            return -2;
                        }

                        fputs(entete, fout);
                    }
                }
            }
        }
    }
    while (!feof(f));

    fclose (f);
    fclose (fout);

    return 0;
}

#include <stdio.h>
#include <stdlib.h>

/* DECLARACAO DOS PROTOTIPOS */
int *load_image_from_file(char *filename, int *w, int *h);
void save_image_to_file(char *filename, int *I, int w, int h);

int *invert_image(int *I, int w, int h);
int *binarize(int *I, int w, int h, int t);
int *filter_image(int *I, int w, int h, int d, int tipo);
int *outline_image(int *I, int w, int h);
int *label_components(int *I, int w, int h, int *ncc);

void insertion_sort (int *V, int n);
void color(int *IC, int *RC, int i, int j, int wc, int hc, int *c);
void transfer_image(int *I, int *R, int w, int h);

/* EXECUCAO DO PROGRAMA */
int main()
{
    int execute, w, h, t, tipo, d, ncc;
    int *I, *R;

    char option;
    
    char filename[50];

    execute = 1;

    while (execute)
    {
        printf("----------------------------------------------\n");
        printf("Escolha uma opcao\n");
        printf("  Q. Quit\n");
        printf("  L. Ler um arquivo\n");
        printf("  S. Salvar imagem em arquivo\n");
        printf("  M. Manter entrada anterior\n\n");
        printf("  B. Binarizar a imagem\n");
        printf("  C. Contorno da imagem\n");
        printf("  I. Inverter a imagem\n");
        printf("  F. Filtrar a imagem\n");
        printf("  R. Rotular os componentes conexos\n");
        printf(" ==> ");
        scanf(" %c", &option);
        printf("\n");


        /* OPCOES */
        if (execute == 1)
        {
            if (option == 'q')
                return 0;

            else if (option == 'l')
                I = load_image_from_file(filename, &w, &h);

            execute = 2;
        }

        else if (execute == 2)
        {
            if (option == 'q')
            {
                free(I); I = NULL;
                return 0;
            }

            else if (option == 'l')
            {
                free(I); I = NULL;
                I = load_image_from_file(filename, &w, &h);
            }

            else if (option == 's')
                save_image_to_file(filename, I, w, h);

            else
            {
                if (option == 'b')
                {
                    printf(" ==> Digite o limiar: ");
                    scanf("%d", &t);
                    printf("\n");

                    R = binarize(I, w, h, t);

                    printf(" ** Imagem foi binarizada\n\n");
                }

                else if (option == 'c')
                {
                    R = outline_image(I, w, h);

                    printf(" ** Contorno foi calculado\n\n");
                }

                else if (option == 'f')
                {
                    printf(" ==> Digite o tipo do filtro: ");
                    scanf("%d", &tipo);
                    printf(" ==> Digite o tamanho do filtro: ");
                    scanf("%d", &d);

                    R = filter_image(I, w, h, d, tipo);

                    printf(" ** Imagem foi filtrada\n\n");
                }

                else if (option == 'i')
                {
                    R = invert_image(I, w, h);

                    printf(" ** Imagem foi invertida\n\n");
                }

                else if (option == 'r')
                {
                    ncc = 0;
                    R = label_components(I, w, h, &ncc);

                    printf(" ** Imagem foi rotulada\n");
                    printf(" ** Numero de componentes conexos: %d\n\n", ncc);
                }

                execute = 3;
            } 
        }

        else if (execute == 3)
        {
            if (option == 'q')
            {
                free(R); R = NULL; free(I); I = NULL;
                return 0;
            }

            else if (option == 'l')
            {
                free(I); I = NULL; free(R); R = NULL;
                I = load_image_from_file(filename, &w, &h);
            }

            else if (option == 's')
                save_image_to_file(filename, R, w, h);

            else if (option == 'm')
            {
                free(R); R = NULL;
                printf(" ** Imagem entrada anterior foi recuperada\n\n");
                execute = 2;
            }

            else
            {
                transfer_image(I, R, w, h);
                free(R); R = NULL;

                if (option == 'b')
                {
                    printf(" ==> Digite o limiar: ");
                    scanf("%d", &t);
                    printf("\n");

                    R = binarize(I, w, h, t);

                    printf(" ** Imagem foi binarizada\n\n");
                }

                else if (option == 'c')
                {
                    R = outline_image(I, w, h);

                    printf(" ** Contorno foi calculado\n\n");
                }

                else if (option == 'f')
                {
                    printf(" ==> Digite o tipo do filtro: ");
                    scanf("%d", &tipo);
                    printf(" ==> Digite o tamanho do filtro: ");
                    scanf("%d", &d);

                    R = filter_image(I, w, h, d, tipo);

                    printf(" ** Imagem foi filtrada\n\n");
                }

                else if (option == 'i')
                {
                    R = invert_image(I, w, h);

                    printf(" ** Imagem foi invertida\n\n");
                }

                else if (option == 'r')
                {
                    ncc = 0;
                    R = label_components(I, w, h, &ncc);

                    printf(" ** Imagem foi rotulada\n");
                    printf(" ** Numero de componentes conexos: %d\n\n", ncc);
                }
            }
        }

    }

    return 0;
}



/* FUNCOES */
int *load_image_from_file(char *filename, int *w, int *h)
{
    FILE *arq_entrada;
    int *I;
    int i, j;
    char line[10];

    printf(" ==> Digite o nome do arquivo de entrada: ");
    scanf("%s", filename);
    arq_entrada = fopen(filename, "r");

    if (arq_entrada == NULL)
    {
        printf("Nao consegui abrir o arquivo %s\n\n", filename);
        return 0;
    }

    else
    {
        fscanf(arq_entrada, "%[^\n]\n", line);
        fscanf(arq_entrada, "%d %d", w, h);
        fscanf(arq_entrada, "%[^\n]\n", line);

        printf(" ** Tamanho da imagem: %d x %d\n", *w, *h);
        printf("\n");

        I = malloc(sizeof(int) * (*w) * (*h));

        for (i = 0; i < (*h); i++)
            for (j = 0; j < (*w); j++)
                fscanf(arq_entrada, "%d", &I[(*w)*i + j]);

        fclose(arq_entrada);
            
        return I;
    }
}

void save_image_to_file(char *filename, int *I, int w, int h)
{
    int i, j;
    char line[10];
    FILE *arq_saida;

    printf(" ==> Digite o nome do arquivo de saída: ");
    scanf("%s", filename);
    arq_saida = fopen(filename, "w");

    if (arq_saida == NULL)
        printf("Nao consegui abrir o arquivo %s\n", filename);

    else
    {
        fprintf(arq_saida, "P2\n");
        fprintf(arq_saida, "%d %d\n", w, h);
        fprintf(arq_saida, "255\n");

        for (i = 0; i < h; i++)
        {
            for (j = 0; j < w; j++)
            {
                if (j == w - 1)
                    fprintf(arq_saida, "%d", I[w*i + j]);
                else
                    fprintf(arq_saida, "%d ", I[w*i + j]);
            }
            fprintf(arq_saida, "\n");
        }

        fclose(arq_saida);

        printf(" ** Imagem gravada no arquivo %s\n\n", filename);
    }
}

int *invert_image(int *I, int w, int h)
{
    int i, j;
    int *R;

    R = malloc(sizeof(int) * w * h);

    for (i = 0; i < h; i++)
        for (j = 0; j < w; j++)
            R[w*i + j] = 255 - I[w*i + j];

    return R;
}

int *binarize(int *I, int w, int h, int t)
{
    int i, j;
    int *R;

    R = malloc(sizeof(int) * w * h);

    for (i = 0; i < h; i++)
        for (j = 0; j < w; j++)
        {
            if (I[w*i + j] >= t)
                R[w*i + j] = 255;
            else
                R[w*i + j] = 0;
        }
    
    return R;
}

int *filter_image(int *I, int w, int h, int d, int tipo)
{
    int *C; /* Matriz copia com borda */
    int *D; /* Matriz dxd */
    int *R;
    int i, ii, j, jj, k, borda;

    borda = d/2;

    C = malloc(sizeof(int) * (h + 2*borda) * (w + 2*borda));

    /* Montagem da matriz copia */
    /* Canto superior esquerdo */
    for (i = 0; i < borda; i++)
        for (j = 0; j < borda; j++)
            C[w*i + j] = I[0]; /* I[0][0] -> elemento do canto superior esquerdo de I */

    /* Canto superior direito */
    for (i = 0; i < borda; i++)
        for (j = w + borda; j < w + 2*borda; j++)
            C[w*i + j] = I[w-1]; /* I[0][w-1] -> elemento do canto superior direito de I  */

    /* Canto inferior esquerdo */
    for (i = h + borda; i < h + 2*borda; i++)
        for (j = 0; j < borda; j++)
            C[w*i + j] = I[w*(h-1)]; /* I[h-1][0] -> elemento do canto inferior esquerdo de I */

    /* Canto inferior direito */
    for (i = h + borda; i < h + 2*borda; i++)
        for (j = w + borda; j < w + 2*borda; j++)
            C[w*i + j] = I[w*(h-1) + w-1]; /* I[h-1][w-1] -> elemento do canto inferior direito de I */

    /* Linhas-borda superiores */
    for (i = 0; i < borda; i++)
        for (j = borda; j < w + borda; j++)
            C[w*i + j] = I[j-borda]; /* I[0][j-borda] -> elementos da primeira linha de I */
    
    /* Linhas-borda inferiores */
    for (i = h + borda; i < h + 2*borda; i++)
        for (j = borda; j < w + borda; j++)
            C[w*i + j] = I[w*(h-1) + j-borda]; /* I[h-1][j-borda] -> elementos da ultima linha de I */

    /* Colunas-borda a esquerda */
    for (j = 0; j < borda; j++)
        for (i = borda; i < h + borda; i++)
            C[w*i + j] = I[w*(i-borda)]; /* I[i-borda][0] -> elementos da primeira coluna de I */

    /* Colunas-borda a direita */
    for (j = w + borda; j < w + 2*borda; j++)
        for (i = borda; i < h + borda; i++)
            C[w*i + j] = I[w*(i-borda) + w-1]; /* I[i-borda][w-1] -> elementos da ultima coluna de I */

    /* Miolo */
    for (i = borda; i < h + borda; i++)
        for (j = borda; j < w + borda; j++)
            C[w*i + j] = I[w*(i-borda) + j-borda];

    /* Filtragem*/
    R = malloc(sizeof(int) * w * h);
    D = malloc(sizeof(int) * d * d);

    for (i = borda; i < h + borda; i++)
    {
        for (j = borda; j < w + borda; j++)
        {
            /* Leitura da matriz d x d */
            k = 0;
            for(ii = i - borda; ii <= i + borda; ii++)
            {
                for(jj = j - borda; jj <= j + borda; jj++)
                {
                    D[k] = C[w*ii + jj];
                    k++;
                }
            }

            insertion_sort(D, d*d);

            if (tipo == 1)
                R[w*(i-borda) + j-borda] = D[0];
            else if (tipo == 2)
                R[w*(i-borda) + j-borda] = D[(d*d - 1) / 2];
            else if (tipo == 3)
                R[w*(i-borda) + j-borda] = D[d*d - 1];
        }
    }

    free(C); C = NULL;
    free(D); D = NULL;
    
    return R;
}

int *outline_image(int *I, int w, int h)
{
    int i, j;
    int *R, *F;

    R = malloc(sizeof(int) * w * h);

    F = filter_image(I, w, h, 3, 1);

    for (i = 0; i < h; i++)
        for (j = 0; j < w; j++)
            R[w*i + j] = I[w*i + j] - F[w*i + j];

    free(F); F = NULL;

    return R;
}

int *label_components(int *I, int w, int h, int *ncc)
{
    /* Pressupoe-se que o usuario ja binarizou a imagem */

    int i, j, wc, hc;
    int *IC; /* Matriz I com borda 0 */
    int *RC; /* Matriz R com borda*/
    int *R;

    /* Montagem da matriz IC */
    wc = w + 2; hc = h + 2;
    IC = malloc(sizeof(int) * wc * hc);

    for (i = 0; i < hc; i++)
    {
        for (j = 0; j < wc; j++)
        {
            if ((i == 0 || i == hc - 1) && (j == 0 || j == wc - 1))
                IC[wc*i + j] = 0;
            else
                IC[wc*i + j] = I[w*(i-1) + j-1];
        }
    }

    /* Setando a matriz RC */
    RC = malloc(sizeof(int) * wc * hc);
    for (i = 0; i < hc; i++)
        for (j = 0; j < wc; j++)
            RC[wc*i + j] = 0;

    /* Algoritmo recursivo*/
    for (i = 1; i <= h; i++)
        for (j = 1; j <= w; j++)
            if (IC[wc*i + j] && !RC[wc*i + j])
            {
                *ncc += 1;
                color(IC, RC, i, j, wc, hc, ncc);
            }

    /* Diferenciacao de RC */
    for (i = 1; i <= h; i++)
        for (j = 1; j <= w; j++)
            if(RC[wc*i + j] > 0 && RC[wc*i + j] <= 127)
                RC[wc*i + j] *= 255 / RC[wc*i + j];

    /* Setando a matriz R */
    R = malloc(sizeof(int) * w * h);
    for (i = 1; i <= h; i++)
        for (j = 1; j <= w; j++)
            R[w*(i-1) + j-1] = RC[wc*i + j];

    free(IC); IC = NULL; free(RC); RC = NULL;

    return R;
}

void color(int *IC, int *RC, int i, int j, int wc, int hc, int *c)
{
    int cont, ni, nj;
    int di[8] = {1,1,1,0,-1,-1,-1,0};
    int dj[8] = {-1,0,1,1,1,0,-1,-1};

    RC[wc*i + j] = *c;

    for (cont = 0; cont < 8; cont++)
    {
        ni = i + di[cont];
        nj = j + dj[cont];

        if (IC[wc*ni + nj] && !RC[wc*ni + nj])
            color(IC, RC, ni, nj, wc, hc, c);
    }
}

void insertion_sort(int *V, int n)
{
    int i, j, aux;

    for (i = 1; i < n; i++)
    {
        j = i - 1;
        aux = V[i];

        while (j >= 0 && V[j] > aux)
        {
            V[j+1] = V[j];
            j--;
        }

        V[j+1] = aux;
    }
}

void transfer_image(int *I, int *R, int w, int h)
{
    int i, j;

    for (i = 0; i < h; i++)
        for (j = 0; j < w; j++)
            I[w*i + j] = R[w*i + j];
}


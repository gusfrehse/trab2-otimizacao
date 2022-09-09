#include <iostream>
#include <vector>
#include <functional>
#include <unistd.h>

#define N 10000

#define inf int(1e9)

// resposta atual
std::vector<int> X(N);
std::vector<int> grupos(N, 0);
std::vector<bool> atores_escolhidos(N, 0);
int custo_atual = 0;

// resposta otima
std::vector<int> resposta_otima(N);
int custo_otimo = inf;

// instancia
int num_grupos;
int num_personagens;
int num_atores;
std::vector<std::vector<int>> grupos_ator(N, std::vector<int>(N));
std::vector<int> tamanho_grupos(N);
std::vector<int> valor_ator(N);

std::function<int(int)> b;

bool viavel(int l) {

    // número de atores escolhido precisa ser igual ao número de personagens.
    if (l != num_personagens)
        return false;

    // checa para ver se existe pelo menos 1 grupo que nao tenha representantes.
    for (int i = 0; i < num_grupos; i++) {
        if (grupos[i] == 0) {
            return false;
        }
    }

    return true;
}

int bound_dada(int l) {
    int sum = 0;
    int menor_valor = -inf;

    for (int i = 0; i < num_atores; i++) {
        if (atores_escolhidos[i]) {
            sum += valor_ator[i];
        } else {
            if (valor_ator[i] < menor_valor)
                menor_valor = valor_ator[i];
        }
    }

    sum += (num_personagens - l) * menor_valor;

    return sum;
}

void bb(int l) {
#ifdef DEBUG
    std::cout << "bb(" << l << ") custo: " << custo_atual << " X = ";

    for (int i = 0; i < l; i++)
    {
        std::cout << X[i] << " ";
    }

    std::cout << "\n";

    std::cout << "  os grupos representados atualmente: {\n";
    for (int i = 0; i < num_grupos; i++) {
        std::cout << "    " << i << " com " << grupos[i] << " atores\n";
    }
    std::cout << "  }\n";
#endif

    if (viavel(l)) {
#ifdef DEBUG
        std::cout << "  viavel\n";
#endif
        std::cout << custo_atual <<  " folha -> " << l << ": ";
        for (int i = 0; i < l; i++)
        {
            std::cout << X[i] << " ";
        }

        std::cout << "\n";

        if (custo_atual < custo_otimo) {
            custo_otimo = custo_atual;
            resposta_otima = X;
        }
    }
#ifdef DEBUG
    else {
        std::cout << "  inviavel\n";
    }
#endif


    if (l == num_personagens) {
        // não é possível ter mais soluções validas a partir daqui, já que o
        // número de personagens só aumenta.
        return;
    }

    // calcular bound
    int bound = b(l);

    for (int i = 0; i < num_atores; i++) {
        // checa se:
        //   a) ator i nao foi escolhido.
        //   b) ator i vem depois do ultimo ator escolhido (serve para não
        //      escolhermos subconjuntos iguais).
        //   c) bound ainda é melhor que o ótimo atual (já que o ótimo pode ter
        //      melhorado na iteração anterior).
        if (!atores_escolhidos[i] &&
                (i > X[l - 1] || l == 0) &&
                bound <= custo_otimo) {

            // adiciona ator aos escolhidos
            atores_escolhidos[i] = 1;

            // adiciona ele ao vetor resposta
            X[l] = i;

            // adiciona os grupos os quais o ator participa
            for (int j = 0; j < tamanho_grupos[i]; j++) {
                grupos[grupos_ator[i][j]]++;
            }

            // adiciona o valor dele ao custo atual
            custo_atual += valor_ator[i];

            // escolhe proximo ator
            bb(l + 1);

            // remove o valor dele do custo atual
            custo_atual -= valor_ator[i];

            // remove os grupos que o autor participa
            for (int j = 0; j < tamanho_grupos[i]; j++) {
                grupos[grupos_ator[i][j]]--;
            }

            // retira ator dos escolhidos
            atores_escolhidos[i] = 0;
        }
    }
}

int main(int argc, char* argv[]) {

    int opt;

    bool cortes_otimalidade = true;
    bool cortes_viabilidade = true;

    b = bound_dada;

    while ((opt = getopt(argc, argv, "foa")) != -1) {
        switch (opt) {
            case 'f':
                cortes_viabilidade = false;
                break;
            case 'o':
                cortes_otimalidade = false;
                break;
            case 'a':
                // TODO: mudar funcao para a dada pelos professores
                break;
            default:
                break;
        }

    }

    std::cin >> num_grupos >> num_atores >> num_personagens;

    for (int i = 0; i < num_atores; i++) {

        std::cin >> valor_ator[i] >> tamanho_grupos[i];


        for (int j = 0; j < tamanho_grupos[i]; j++) {
            std::cin >> grupos_ator[i][j];
            grupos_ator[i][j]--; // indexacao por 0
        }
    }

#ifdef DEBUG
    for (int i = 0; i < num_atores; i++) {
        std::cout << "Ator " << i << " custa " << valor_ator[i]
            << " e faz parte de " << tamanho_grupos[i] << " grupos:\n";

        for (int j = 0; j < tamanho_grupos[i]; j++) 
            std::cout << "\t" << grupos_ator[i][j] << "\n";
    }
#endif

    bb(0);

    if (custo_otimo == inf) {
        std::cout << "inviavel\n";
    } else {

        for (int i = 0; i < num_personagens - 1; i++) {
            std::cout << resposta_otima[i] + 1 << " ";
        }

        std::cout << resposta_otima[num_personagens - 1] + 1
            << "\n" << custo_otimo << "\n";
    }
}

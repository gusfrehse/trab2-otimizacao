#include <iostream>
#include <vector>
#include <unistd.h>

int main(int argc, char* argv[]) {
  int opt;

  bool cortes_otimalidade = true;
  bool cortes_viabilidade = true;

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

  long long l, m, n;
  std::cin >> l >> m >> n;

  std::vector<long long> v(m);

  for (int i = 0; i < m; i++) {
    long long s;

    std::cin >> v[i] >> s;

    for (int j = 0; j < s; j++) {
      // TODO
    }
  }

}

#include <lammpstrj/lammpstrj.hpp>
#include <numeric>
#include <vector>
class DropletClusterCounter {
private:
  std::string filename_;
  DropletClusterCounter(const std::string &filename) {
    filename_ = filename;
  }

  int find(int index, std::vector<int> &cluster) {
    while (index != cluster[index]) {
      index = cluster[index];
    }
    return index;
  }

  void unite(int i1, int i2, std::vector<int> &cluster) {
    i1 = find(i1, cluster);
    i2 = find(i2, cluster);
    if (i1 < i2) {
      cluster[i2] = i1;
    } else {
      cluster[i1] = i2;
    }
  }

  void clustering(std::vector<lammpstrj::Atom> &atoms) {
    int number_of_atoms = static_cast<int>(atoms.size());
    std::vector<int> cluster(number_of_atoms);
    std::iota(cluster.begin(), cluster.end(), 0);
    const double cutoff = 2.5;
    const double c2 = cutoff * cutoff;
    for (int i = 0; i < number_of_atoms - 1; i++) {
      double xi = atoms[i].x;
      double yi = atoms[i].y;
      double zi = atoms[i].z;
      for (int j = i + 1; j < number_of_atoms; j++) {
        double xj = atoms[j].x;
        double yj = atoms[j].y;
        double zj = atoms[j].z;
        double r2 = (xi - xj) * (xi - xj) + (yi - yj) * (yi - yj) + (zi - zj) * (zi - zj);
        if (r2 > c2) continue;
        unite(i, j, cluster);
      }
    }
    for (int i = 0; i < number_of_atoms; i++) {
      cluster[i] = find(i, cluster);
      printf("%d %d\n", i, cluster[i]);
    }
    exit(1);
  }

  void count_cluster(const std::unique_ptr<lammpstrj::SystemInfo> &si,
                     std::vector<lammpstrj::Atom> &atoms) {
    clustering(atoms);
  }

public:
  static std::unique_ptr<DropletClusterCounter> create(const std::string &filename) {
    auto si = lammpstrj::read_info(filename);
    if (!si) {
      std::cerr << "Error: Could not read file: " << filename << std::endl;
      return nullptr;
    }
    return std::unique_ptr<DropletClusterCounter>(new DropletClusterCounter(filename));
  }
  void calculate() {
    lammpstrj::for_each_frame(filename_,
                              [this](const std::unique_ptr<lammpstrj::SystemInfo> &si, std::vector<lammpstrj::Atom> &atoms) {
                                this->count_cluster(si, atoms);
                              });
  }
};

int main(int argc, char *argv[]) {
  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " input.lammpstrj" << std::endl;
    return 1;
  }
  auto counter = DropletClusterCounter::create(argv[1]);
  counter->calculate();
}

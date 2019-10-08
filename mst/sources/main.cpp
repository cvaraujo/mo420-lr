#include "headers/Data.h"
#include "headers/Model.h"

int main(int argc, const char *argv[]) {
    if (argc < 4) {
        auto *data = new Data("Instances/Washington-50-10/10/final-washington-50-10-10.txt");
        //data->showData();
        //getchar();
        auto *model = new Model(data);
        clock_t t0 = clock();
        model->lagrangean();
        double endTime = double(clock() - t0) / double(CLOCKS_PER_SEC);
//        model->showSolution("EX", "results.txt", 100, 500, 5, endTime);
        return 0;
    } else {
        auto *data = new Data(argv[1]);
        auto *model = new Model(data);
        clock_t t0 = clock();
        model->lagrangean();
        double endTime = double(clock() - t0) / double(CLOCKS_PER_SEC);
//        model->showSolution(argv[1], "results_lagrangean_SHP.txt", atoi(argv[3]), atoi(argv[4]), atoi(argv[5]), endTime);

    }

    return 0;
}

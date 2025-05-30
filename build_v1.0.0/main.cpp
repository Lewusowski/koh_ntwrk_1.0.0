
#include "koh_ntwrk.h"

int main() {

    setlocale(LC_ALL, "");

    int clusters{};
    double sum_prev = -1;
    double  dif = 1000;

    cout << "Введи кол-во кластеров: ";
    cin >> clusters;

    vector<vector<int>>pers_inclust{};
    vector<vector<double>>init_clusts{};
    vector<vector<double>>changed_clusts{};

    vector<vector<int>>fin_pers_inclust{};
    vector<vector<double>>fin_init_clusts{};
    vector<vector<double>>fin_changed_clusts{};

    ofstream file{ "D:/pythonProject7/wcss.txt" };

    for (int i = 1; i < clusters;i++) {

        cout << "\nРаботаем с " << i << " кластами\n";
        double sum_pres = clustering(i, pers_inclust, init_clusts, changed_clusts);
        cout << sum_prev << " " << sum_pres << " " << dif << " " << fabs(sum_prev - sum_pres) << endl;
        file << sum_pres << endl;
        if (sum_prev == -1) sum_prev = sum_pres;

        else if (sum_pres == -1 || i != 2 && (fabs(sum_prev - sum_pres) / dif) < 0.1) {

            cout << "ОПТИМАЛЬНОЕ: " << i - 1 << endl;

            writeinfile("D:/pythonProject7/clusters.txt", fin_init_clusts);
            writeinfile("D:/pythonProject7/changed_clusters.txt", fin_changed_clusts);

            denormalize(fin_pers_inclust, fin_changed_clusts);

            break;
        }

        dif = fabs(sum_prev - sum_pres);
        sum_prev = sum_pres;
        fin_pers_inclust = pers_inclust;
        fin_init_clusts = init_clusts;
        fin_changed_clusts = changed_clusts;
    }

    file.close();

    return 0;
}
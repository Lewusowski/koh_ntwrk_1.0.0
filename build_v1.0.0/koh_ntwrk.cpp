#include <iostream>
#include <filesystem>
#include <vector>
#include <fstream>
#include <string>
#include <random>

using namespace std;

random_device rd;

mt19937 gen(rd());

vector<vector<string>>get_answer_list() {
    vector<vector<string>>answers{};
    string line{};

    fstream file("answerlist.txt");

    if (file.is_open()) {
        vector<string>temp_answers{};
        while (getline(file, line)) {
            if (line.size() != 0) temp_answers.push_back(line);
            else { answers.push_back(temp_answers); temp_answers = {}; }
        }
    }

    file.close();

    return answers;
}

vector<vector<int>>dispersion(vector<vector<tuple<int, int, double>>>group_cluster, vector<vector<int>>& inclust) {

    for (const auto& inner : group_cluster) {
        double disp = 0;
        int group_size = 0;
        for (const auto& vect : inner) {
            auto [x, y, z] = vect;
            disp += z;
            group_size += 1;
        }

        disp = disp * 1.0 / group_size;

        for (const auto& vect : inner) {
            auto [x, y, z] = vect;

            if (z <= 2 * sqrt(disp)) {
                inclust[y].push_back(x);

            }
        }
    }
    return inclust;
}

vector<vector<int>> check_precisison(vector<vector<double>>& norm_vect, vector<vector<double>>& clusters) {

    vector<vector<tuple<int, int, double>>>group_cluster{ clusters.size() };
    vector<vector<int>>inclust{ clusters.size() };


    for (int i = 0; i < norm_vect.size(); i++) {
        if (norm_vect[i].size() == 0) {
            continue;
        }
        double distance_ndpw = 500;
        int clust = 0;
        for (int j = 0; j < clusters.size(); j++) {
            if (clusters[j].size() == 0) {
                continue;
            }

            double clust_dist = 0;
            for (int k = 0; k < norm_vect[i].size(); k++) {
                clust_dist += pow(norm_vect[i][k] - clusters[j][k], 2);
            }

            if (clust_dist < distance_ndpw) {
                distance_ndpw = clust_dist;
                clust = j;
            }
        }

        group_cluster[clust].emplace_back(i, clust, distance_ndpw);
    }

    for (const auto& inner : group_cluster) for (const auto& vect : inner) auto [x, y, z] = vect;

    return dispersion(group_cluster, inclust);
}


vector<int> ind_in_poll_resp(string response) {

    vector<vector<string>>answer_list = get_answer_list();

    vector<int>elem_and_max{};

    for (int i = 0; i < answer_list.size(); i++) {
        for (int j = 0; j < answer_list[i].size(); j++) {
            if (answer_list[i][j] == response) {
                elem_and_max.push_back(j);
                elem_and_max.push_back(answer_list[i].size() - 1);
                return elem_and_max;
            }
        }
    }
}

void writeinfile(string destination, vector<vector<double>>data) {

    ofstream file;

    file.open(destination, ios::ate);

    for (int i = 0; i < data.size(); i++) {
        for (int j = 0; j < data[i].size(); j++) {
            file << data[i][j] << " ";
        }

        file << endl;
    }
}

vector<vector<double>> norm_resp(vector<vector<string>>data) {

    vector<vector<double>>fin_norm_resp{};
    for (auto i : data) {
        vector<double>temp_norm{};
        for (string j : i) {
            if (j.find('\n') != string::npos) {
                j = j.substr(0, j.find('\n'));
            }

            if (j == "") continue;

            vector<int> charcs_pos = ind_in_poll_resp(j);

            double norm_charch = (charcs_pos[0] * 1.0 - 0) / (charcs_pos[1] - 0);
            temp_norm.push_back(norm_charch);
        }

        fin_norm_resp.push_back(temp_norm);

    }

    writeinfile("D:/pythonProject7/dataset.txt", fin_norm_resp);

    return fin_norm_resp;
}

vector<double> create_cluster() {

    vector<double>cluster{};

    uniform_real_distribution<>distribution(0.1, 0.3);

    for (int i = 0; i < 12; i++) {

        double elem = distribution(gen);
        elem = round(elem * 100) / 100;
        cluster.push_back(elem);
    }

    return cluster;
}

int min_elem(vector<double>values) {

    int min_ind = 0;

    for (int i = 1; i < values.size() - 1; i++) {
        if (values[i] < values[min_ind]) min_ind = i;
    }
    return min_ind;
}

int denormalize(vector<vector<int>>fin_clusts, vector<vector<double>>clusters) {


    vector<vector<string>>answer_list = get_answer_list();
    vector<int>main_clust{};

    vector<vector<string>>fin_charcs{};

    for (int i = 0;i < fin_clusts.size();i++) {
        if (fin_clusts[i].size() == 0) continue;
        main_clust.push_back(i);
        vector<string>temp_charcs;
        for (int j = 0;j < clusters[0].size();j++) {
            double resp_ind = round(clusters[i][j] * answer_list[j].size());
            if (resp_ind >= answer_list[j].size()) temp_charcs.push_back(answer_list[j][int(resp_ind) - 1]);
            else temp_charcs.push_back(answer_list[j][resp_ind]);



        }
        fin_charcs.push_back(temp_charcs);
    }

    cout << "\nÕàðàêòåðèñòèêà ëþäåé: \n";
    for (int i = 0;i < fin_charcs.size();i++) {
        cout << "\nÊËÀÑÒÅÐ: " << main_clust[i] << endl;
        cout << endl;
        for (string str : fin_charcs[i]) cout << str << endl;
        cout << endl;
    }

    return 0;
}

int koh_network(vector<vector<double>>& norm_vect, vector<vector<double>>& clusters, int iter) {

    for (int i = 0; i < norm_vect.size(); i++) {

        vector<double>min_distance{};

        int near_nrn_ind{};


        for (int j = 0; j < clusters.size(); j++) {
            double distance = 0;
            for (int k = 0; k < norm_vect[0].size(); k++) {
                distance += pow(norm_vect[i][k] - clusters[j][k], 2);
            }
            min_distance.push_back(sqrt(distance));

        }

        near_nrn_ind = min_elem(min_distance);

        for (int g = 0; g < norm_vect[0].size(); g++) {
            clusters[near_nrn_ind][g] += (0.3 * exp(-iter / 100.0)) * (norm_vect[i][g] - clusters[near_nrn_ind][g]);
        }

    }

    return 0;
}

double euclidean_distance(vector<double>data1, vector<double>data2) {
    double res = 0;
    for (int i = 0;i < data1.size();i++) {
        res += pow(data1[i] - data2[i], 2);
    }
    return sqrt(res);
}

double clustering(int clusters, vector<vector<int>>& pers_inclust, vector<vector<double>>& init_clusts, vector<vector<double>>& changed_clust) {

    vector<vector<double>>clusters_arr{};

    vector<vector<double>>clusters_arr_nd{};

    namespace fs = std::filesystem;

    string directory_path = "C:\\data";

    int a = 0;

    if (fs::exists(directory_path)) {

        vector<vector<string>>data{};

        for (const auto& entry : fs::directory_iterator(directory_path)) {

            vector<string>edited_data{};

            string data_file = entry.path().string();

            fstream file;

            file.open(data_file);

            if (file.is_open()) {

                for (int i = 0; i <= 13; i++) {

                    string temp_data{};

                    getline(file, temp_data, ';');

                    edited_data.push_back(temp_data);
                }

                data.push_back(edited_data);
            }
        }
        vector<vector<double>>norm_values = norm_resp(data);

        cout << "Èíôîðìàöèÿ îáðàáîòàíà" << endl;

        for (int i = 0; i < clusters; i++) {

            vector<double>cluster = create_cluster();
            clusters_arr.push_back(cluster);
            clusters_arr_nd.push_back(cluster);
        }

        for (int i = 0; i < 100; i++) koh_network(norm_values, clusters_arr, i);

        vector<vector<int>>disp_res = check_precisison(norm_values, clusters_arr);

        vector<vector<int>>fin_disp_res{ clusters_arr.size() };

        int peop_am = 0;

        double sum_main{};

        while (true) {
            double sum = 0;
            vector<int>del_clust{};

            vector<vector<int>>disp_res = check_precisison(norm_values, clusters_arr);

            for (int i = 0;i < disp_res.size();i++) {
                if (disp_res[i].size() != 0) {
                    fin_disp_res[i].append_range(disp_res[i]);
                    for (int j = 0;j < disp_res[i].size();j++) {
                        sum += euclidean_distance(norm_values[disp_res[i][j]], clusters_arr[i]);
                        norm_values[disp_res[i][j]].clear();
                        peop_am++;

                    }
                }
                else return -1;
            }

            if (peop_am == norm_values.size()) {
                sum_main = sum;
                break;
            }

        }

        pers_inclust = fin_disp_res;
        init_clusts = clusters_arr_nd;
        changed_clust = clusters_arr;

        return sum_main;
    }
}

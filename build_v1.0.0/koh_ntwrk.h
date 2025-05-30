#pragma once
#include <iostream>
#include <filesystem>
#include <vector>
#include <fstream>
#include <string>
#include <random>

using namespace std;

vector<vector<string>>get_answer_list();

vector<vector<int>>dispersion(vector<vector<tuple<int, int, double>>>, vector<vector<int>>&);

vector<vector<int>> check_precisison(vector<vector<double>>&, vector<vector<double>>&);

vector<int> ind_in_poll_resp(string);

void writeinfile(string, vector<vector<double>>);

vector<vector<double>> norm_resp(vector<vector<string>>);

vector<double>create_cluster();

int min_elem(vector<double>);

int denormalize(vector<vector<int>>, vector<vector<double>>);

int koh_network(vector<vector<double>>&, vector<vector<double>>&, int);

double euclidean_distance(vector<double>, vector<double>);

double clustering(int, vector<vector<int>>&, vector<vector<double>>&, vector<vector<double>>&);

/*!
  \file
* В данном файле собраны все константы, используемые в работе программы. Из названий переменных можно понять для чего они.
*/


#ifndef COMMON_H
#define COMMON_H
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <cmath>
#include <map>
#include <future>



namespace PlasmaLab {

    using std::string;
    using std::vector;
    using std::ifstream;
    using std::ofstream;
    using std::fstream;
    using std::cout;
    using std::cin;
    using std::endl;
    using std::ios;
    using std::pair;

    using vec_d  = vector<double>;
    using vvec_d = vector<vector<double>>;

    enum class WorkMode
    {
        no_optimization = 0,
        alternating_variable_descent_method = 1,
        particle_swarm_method = 2,
        method_of_steepest_descent = 3
    };

    const string path_for_input_data                            = "input_data/";
    const string path_for_output_data                           = "output_data/";
    const string file_name_for_currents_result                  = "data_currents.txt";
    const string file_name_for_derivative_currents_result       = "data_derivative_current.txt";
    const string file_name_for_test_currents                    = "j.dat";
    const string file_name_for_inductance                       = "matr_ind_univ.dat";
    const string file_name_for_resistance                       = "matr_res_univ.dat";
    const string file_name_for_required_current_plasma          = "Ip.dat";
    const string file_name_for_alfa_z                           = "bz_cache.dat";
    const string file_name_for_initial_currents                 = "I0.txt";
    const string file_name_for_alfa_psi_alfa_r_alfa_z           = "matr_field_univ.dat";
    const vector<string> file_name_for_coils                    = { "cs3u.txt",
                                                                    "cs2u.txt",
                                                                    "cs1.txt",
                                                                    "cs2l.txt",
                                                                    "cs3l.txt",
                                                                    "pf1.txt",
                                                                    "pf2.txt",
                                                                    "pf3.txt",
                                                                    "pf4.txt",
                                                                    "pf5.txt",
                                                                    "pf6.txt"};

}

#endif // COMMON_H

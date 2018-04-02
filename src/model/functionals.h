/*!
  \file
* В данном файле реализована работа с технологическими, физическими и другими требованиями и ограничениями на модель динамики полоидальных токов в токамаке ITER.
* Меодель представляет собой систему линейных дифф. уравнений 1 порядка составленых по правилу Кирхгофа.
* Результатом являются два контейнера. В одном хранятся значения полоидальных токов в контурах в каждый момент
* времени, в другом - производные этих токов.
*/

#ifndef FUNCTIONALS_H
#define FUNCTIONALS_H

#include "src/read_data/read_data.h"

namespace PlasmaLab {

    /*В классе происходит проверка условий пробоя и вычисление значений интегральных функционалов до Пробоя включительно*/
    class BeforeBD{
    protected:
      IsBreakdown bd_key; ///< ключ, были или не был пробой
      IsRequirements requiments_key; ///< соблюдены ли технологические и физические ограничения на систему
      bool init_key; ///< ключ, если false, значит первая итерация в методе Рунге-Кутта
      double u_loop; ///< напряжение на обходе
      vector<double> r_fields; ///< значения радиальной компоненты магнитного поля в контрольных точках
      vector<double> z_fields; ///< значения вертикальной компоненты магнитного поля в контрольных точках
      vec_d max_currents; ///< максимально допустимые токи в полоидальных катушках
      int number_coils; ///< кол-во полоидальных (управляющих) катушек
      double r_field_max, ///< максимальное значения радиальной компоненты магнитного поля в каждой контрольной точке
             z_field_max, ///< максимальное значения вертикальной компоненты магнитного поля в каждой контрольной точке
             nessesary_u_loop; ///< необходимое для пробоя значение напряжения на обходе контура
    protected:
      void calc_conditions_bd(int point,const vvec_d &currents, const vvec_d &derivative_of_current, const vvec_d &alfa_psi,
                              const vvec_d &alfa_r,const vvec_d &alfa_z);
      void calc_requiments(double prev_u_loop);
    public:
      BeforeBD() = delete;
      /*единственно возможный конструктор*/
      BeforeBD(const ReadData &rd);
      /*проверка условий пробоя, выполнены ли они.*/
      IsBreakdown run(IsRequirements &out_requiments_key,int point,const vvec_d &currents, const vvec_d &derivative_of_current, const vvec_d &alfa_psi,
                                  const vvec_d &alfa_r,const vvec_d &alfa_z);

      /*получить текущее значение напряжения на обходе*/
      double get_u_loop() const;
      /*получить текущее значение радиальной компоненты магнитного поля в контрольных точках*/
      double get_r_fields() const;
      /*получить текущее значение вертикальной компоненты магнитного поля в контрольных точках*/
      double get_z_fields() const;
      /*узнать был пробой или нет*/
      IsBreakdown get_bd_key() const;
    };


    class FunctionalModel{
        enum FuncIdx{
            idx_total = 0, ///< общий функционал (равный сумме всех штрафных функций)
            idx_loop_voltage_before_bd = 1, ///< значений штрафной функции для напряжения на обходе до пробоя
            idx_r_fields = 2, ///< значений штрафной функции для радиальной компоненты маг.поля на всем этапе моделирования
            idx_z_fields_before_bd = 3, ///< значений штрафной функции для вертикальной компоненты маг.поля до пробоя
            idx_mux_flux_before_bd = 4, ///< значений штрафной функции для маг.потока в центре области пробоя до пробоя
            idx_loop_voltage_derivative_before_bd = 5, ///< значений штрафной функции для производной напряжения на обходе до пробоя
            idx_max_currents = 6, ///< значений штрафной функции для максимальных значений токов в полоидальных катушках
            idx_max_voltages = 7, ///< значений штрафной функции для максимальных значений напряжений в полоидальных катушках
            idx_max_res_voltages = 8, ///< значений штрафной функции для резистивных напряжений в полоидальных катушках
            idx_z_fields_after_bd =9 ///< значений штрафной функции для вертикальной компоненты маг.поля в контрольных точках после пробоя
        };
        const uint size_func_idx = 10; ///< общее кол-во штрахных функций +1 общий функионал суммы
        BeforeBD beforeBD;
        IsBreakdown bd_key; ///< был или не был пробой
        IsRequirements requirements_key; ///< соблюдены ли технологические и физические ограничения на систему
        vector<double> weighting_factors; ///< веса для значений штрафных функций (функционалом)
        vector<double> functional_values; ///< значения штрафных функций
    public:
        FunctionalModel() = delete;
        FunctionalModel(const ReadData &rd) : beforeBD(rd){}

        BeforeBD &get_BeforeBD(){ return beforeBD; }
    };
}

#endif // FUNCTIONALS_H

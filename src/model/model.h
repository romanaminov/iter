/*!
  \file
* В данном файле реализована модель динамики полоидальных токов в токамаке ITER.
* Меодель представляет собой систему линейных дифф. уравнений 1 порядка составленых по правилу Кирхгофа.
* Результатом являются два контейнера. В одном хранятся значения полоидальных токов в контурах в каждый момент
* времени, в другом - производные этих токов.
*/
#ifndef MODEL_H
#define MODEL_H

#include "src/read_data/read_data.h"

namespace PlasmaLab {
    void matrix_multiplier(const vec_d &, const vec_d &,vec_d &, int, int);
    void matrix_multiplier(double &, const vec_d &,const vec_d &);

    enum class IsBreakdown
    {
        no  = 0,
        yes = 100
    };

    struct IncrementOfParameters{
        double data;
    };
    /*В классе происходит проверка условий пробоя и вычисление значений интегральных функционалов до Пробоя включительно*/
    class FunctionalsBeforeBreakdown{
      IsBreakdown bd_key;
      bool init_key;
      double u_loop;
      vector<double> r_fields;
      vector<double> z_fields;
      vec_d max_currents;
      int number_coils;

      const double r_field_max,
             z_field_max,
             nessesary_u_loop;
    public:
      FunctionalsBeforeBreakdown() = delete;
      /*единственно возможный конструктор*/
      FunctionalsBeforeBreakdown(const ReadData &rd) : number_coils(rd.get_coils_count()), bd_key(IsBreakdown::no), init_key(false), u_loop(0),
                                nessesary_u_loop(rd.get_required_loop_voltage()),
                                r_field_max(rd.get_r_field_max()), z_field_max(rd.get_z_field_max()) {
          for(int i=0;i < rd.get_control_points();++i){
              r_fields.push_back(0);
              z_fields.push_back(0);
          }
          rd.get_currents_max(max_currents);
      }
      /*проверка условий пробоя, выполнены ли они.*/
      IsBreakdown check_breakdown(int point,const vvec_d &currents, const vvec_d &derivative_of_current, const vvec_d &alfa_psi,
                                  const vvec_d &alfa_r,const vvec_d &alfa_z);

      /*получить текущее значение напряжения на обходе*/
      double get_u_loop() const;
      /*получить текущее значение радиальной компоненты магнитного поля в контрольных точках*/
      double get_r_fields() const;
      /*получить текущее значение вертикальной компоненты магнитного поля в контрольных точках*/
      double get_z_fields() const;
    };
    class Functionals{
        FunctionalsBeforeBreakdown functionalsBeforeBreakdown;
    public:
        Functionals() = delete;
        Functionals(const ReadData &rd) : functionalsBeforeBreakdown(rd){}

        FunctionalsBeforeBreakdown &get_functionalsBeforeBreakdown(){ return functionalsBeforeBreakdown; }
    };

    class Model
    {
    public:
        Model(void);
        ~Model(void);
    private:
        IsBreakdown breakdown_key;              //показывает был или не был пробой плазмы
        WorkMode work_mode;                     //режим работы программы

        int system_size;                        //размерность системы уравнений, до того как возникла плазма
        int coils_count;                        //количество управляющих катушек
        int short_step;                         //чтобы не грузить систему, полученные данные записываем в файл с шагом short_step умноженное на integration_step
        int control_points_count;               //количество контрольных точек на контуре плазмы

        double required_loop_voltage;           //необходимое для пробоя плазмы напряжение на обходе
        double r_field_max,
               z_field_max;                     //максимальные значения радиальной и вертикальной составляющих магнит. поля в контрольных точках в момент пробоя
        double epsilon;                         //погрешность вычислений, используется при оптимизации
        double integration_step;                //шаг интегрирования в методе рунге-кутта 4 порядка
        double work_time;                       //время интегрирования системы (моделирования)
        double breakdown_time;                  //время пробоя
        double number_of_steps_in_method;       //количество шагов в методе Рунге-Кутта 4 порядка точности

        vvec_d voltage_in_coils;                //напряжения в управляющих катушках  на протяжении работы системы (кусочно-линейная функция)
        vvec_d test_currents_in_coils;          //токи в управляющих катушках  на протяжении работы системы (проверочные значения из файла j.dat)
        vvec_d derivative_of_current;           //первые производные силы тока
        vvec_d alfa_r;                          //константные коэффициенты, необходимые для расчета радиальных компонент магнитного поля
        vvec_d alfa_z;                          //константные коэффициенты, необходимые для расчета вертикальных компонент магнитного поля
        vvec_d alfa_psi;                        //константные коэффициенты, необходимые для расчета магнитного потока в центре области пробоя
        vvec_d currents;                        //полоидальные токи в контурах на всем промежутке моделирования

        vec_d voltages_in_some_momente;         //напряжение в какой-то момент времени
        vec_d voltages_max;                     //максимальные значения напряжений в полоидальных(управляющих) катушках  на протяжении работы системы
        vec_d currents_max;                     //максимальные значения токов в полоидальных(управляющих) катушках  на протяжении работы системы
        vec_d mag_fields_max;                   //максимальные значения магнитных полей в полоидальных(управляющих) катушках на протяжении работы системы
        vec_d weight_coefficient;               //весовые коэффициенты для функционалов
        vec_d required_current_plasma;          //заданный режим поднятия тока плазмы
        vec_d inductance_matrix;                //матрица индуктивностей, собственных и взаимных
        vec_d inverse_inductance_matrix;        //обратная матрица индуктивностей, собственных и взаимных
        vec_d plasma_inductance_matrix;         //матрица взаимных индуктивностей плазмы в др. контурами
        vec_d resistance_matrix;                //сопротивления в катушках

        vec_d inverse_L_on_R_matrix;            //матрица от умножения обратной матрицы индуктивности на матрицу сопротивлений
        vec_d inverse_L_on_Mp_matrix;           //матрица от умножения обратной матрицы индуктивности на взаимные сопротивления плазмы с др.элементами камеры

        inline void data_preparation(const ReadData &);
        /*уравнения Киргкофа. система ДО и ПОСЛЕ пробоя.*/
        inline void model_combined_equations(double, vec_d &,vec_d &);
        /*вычислить напряжение в катушке в текущий момент времени*/
        inline void voltage_calculator(double);
        /*вычислить значение текущего времени */
        inline int time_comparison(bool &, double, const vec_d &);
        inline int parser_alfa_xxx(const ReadData &);
        /*вычислить обратную матрицу индукнитвностей*/
        inline int slau_gauss(vec_d &, vec_d);
        inline int runge_kutta_4(Functionals &);
        /*вычисление напряжения в катушке в момент времени*/
        inline double dependence_U_on_T(double, double, double, double, double);
        /*кусочно-линейная функция тока плазмы*/
        inline double law_of_plasma_current(double);
     public:
        /*!
         * \brief Главная функция.
         * \param[in,out] ReadData &model
         * \return 0, если все хорошо.
         */
        int main_function(const ReadData &, Functionals &);

        /*!
         * \brief Возвращет посчитанные в каждый момент времени (дискретно) токи в контурах системы.
         * \return const vvec_d &
        */
        inline const vvec_d &get_currents()const {                     return currents; }
        /*!
         * \brief Возвращет посчитанные в каждый момент времени (дискретно) производные токов в контурах системы.
         * \return const vvec_d &
        */
        inline const vvec_d &get_derivative_of_currents()const {       return derivative_of_current; }
        /*!
         * \brief Возвращает размерность системы лин. дифф. уравнений.
         * \return int
        */
        inline int get_system_size() const {                           return system_size; }
        /*!
        чтобы не грузить систему, полученные данные записываем в файл с шагом short_step умноженное на integration_step
        \result int short_step
        */
        inline int get_short_step() const {                            return short_step; }
        /*!
        Возвращет шаг интегрированя системы линейных дифф. уравнений в методе Рунге-Кутта 4 порядка точности
        \result double integration_step
        */
        inline double get_integration_step() const {                   return integration_step; }
    };

}

#endif // MODEL_H

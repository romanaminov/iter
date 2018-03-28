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
    //struct Func

    /*В классе происходит проверка условий пробоя и вычисление значений интегральных функционалов до Пробоя включительно*/
    class FunctionalsBeforeBreakdown{
      IsBreakdown bd_key;
      bool init_key;
      double u_loop;
      vector<double> r_fields;
      vector<double> z_fields;
      vec_d max_currents;
      int number_coils;
      double r_field_max, z_field_max, nessesary_u_loop;
    public:
      FunctionalsBeforeBreakdown() = delete;
      /*единственно возможный конструктор*/
      FunctionalsBeforeBreakdown(const ReadData &rd);
      /*проверка условий пробоя, выполнены ли они.*/
      IsBreakdown run(int point,const vvec_d &currents, const vvec_d &derivative_of_current, const vvec_d &alfa_psi,
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
    class Functionals{
        FunctionalsBeforeBreakdown functionalsBeforeBreakdown;
    public:
        Functionals() = delete;
        Functionals(const ReadData &rd) : functionalsBeforeBreakdown(rd){}

        FunctionalsBeforeBreakdown &get_functionalsBeforeBreakdown(){ return functionalsBeforeBreakdown; }
    };
}

#endif // FUNCTIONALS_H

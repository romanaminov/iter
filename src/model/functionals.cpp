#include "src/model/functionals.h"

namespace  PlasmaLab {
    BeforeBD::BeforeBD(const ReadData &rd) : number_coils(rd.get_coils_count()), bd_key(IsBreakdown::no), init_key(false), u_loop(0),
        nessesary_u_loop(rd.get_required_loop_voltage()),
        r_field_max(rd.get_r_field_max()), z_field_max(rd.get_z_field_max()) {
        for(int i=0;i < rd.get_control_points();++i){
            r_fields.push_back(0);
            z_fields.push_back(0);
        }
    }
    IsBreakdown BeforeBD::run(IsRequirements &out_requiments_key,int point,const vvec_d &currents, const vvec_d &derivative_of_current, const vvec_d &alfa_psi,
                                                       const vvec_d &alfa_r,const vvec_d &alfa_z){

        double prev_u_loop = u_loop;
        calc_conditions_bd(point,currents,derivative_of_current,alfa_psi,alfa_r,alfa_z);

        calc_requiments(prev_u_loop);

        out_requiments_key = requiments_key;

        return bd_key;
    }

    void BeforeBD::calc_conditions_bd(int point,const vvec_d &currents, const vvec_d &derivative_of_current, const vvec_d &alfa_psi,
                            const vvec_d &alfa_r,const vvec_d &alfa_z){
        bd_key = IsBreakdown::yes;
       // double prev_u_loop = u_loop;
        matrix_multiplier(u_loop,derivative_of_current[point], alfa_psi[0]);//напряжение на обходе
        for(uint i = 0;i < r_fields.size(); ++i){ //находим магнитное поле в контрольных точках
            matrix_multiplier(r_fields[i],currents[point], alfa_r[i]);
            matrix_multiplier(z_fields[i],currents[point], alfa_z[i]);
        }
        for(uint i = 0; i < r_fields.size(); ++i){
            if(fabs(r_fields[i])>r_field_max)
                bd_key =  IsBreakdown::no;//если радиальная компонента маг.поля больше МАХ
            if(fabs(z_fields[i])>z_field_max)
                bd_key =  IsBreakdown::no;//если вертикальная компонента маг.поля больше МАХ
        }
        if( ( fabs(u_loop )>(nessesary_u_loop+0.5) ) || ( fabs(u_loop)<(nessesary_u_loop - 0.3) ) )
            bd_key =  IsBreakdown::no; //если напряжение на обходе не равно заданному значению (+/- дельта)

    }

    void BeforeBD::calc_requiments(double prev_u_loop){
        if(bd_key == IsBreakdown::no && init_key == false)
            prev_u_loop = u_loop;//необходимо для вычисления производной напряжения на обходе

        if(bd_key == IsBreakdown::no && prev_u_loop > u_loop) //значит производная напряжения на обходе < 0, что не допустимо
            requiments_key = IsRequirements::no;

        /*for(uint i = 0; i < number_coils; ++i){//токи в катушках должны быть не больше максимальных значений
            if( (currents[point][i] < max_currents[i])&&(currents[point][i] > -max_currents[i]) )
                requiments_key = IsRequirements::yes;
            else
                requiments_key = IsRequirements::no;
        }*/
    }
}

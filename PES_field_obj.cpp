////////////////////////////////////////////////////////
//                        PES                         //
//         written by: Wronsky & Edi Edison           //
////////////////////////////////////////////////////////


#include "PES_field_obj.h"
#include <cmath>

//in the constructors from string, if the string has incomplete information,
//constructor breaks and default values are used for the remaining parameters

void PES_field_obj::set_pos(std::istringstream& data_stream)
{
 PES_vector2d input_pos; 
 data_stream >> input_pos.x;
 if(data_stream.eof()) return;
 data_stream >> input_pos.y;

 this->set_pos(input_pos);
}

void PES_mov_obj::set_vel(std::istringstream& data_stream)
{
 PES_vector2d input_vel;
 data_stream >> input_vel.x; 
 if(data_stream.eof())  return;
 data_stream >> input_vel.y;

 this->set_vel(input_vel);
}

////////////////////////////////////////////////
//                 POINT CHARGE               //
PES_point_charge::PES_point_charge(const std::string data_string)
{
 //default
 charge = 0; image_name = 0;

 std::istringstream data_stream(data_string);
 set_pos(data_stream);
 if(data_stream.eof()) return; 
 data_stream >> charge;
 if(data_stream.eof()) return; 
 data_stream >> image_name;
}


PES_EL_field
PES_point_charge::get_electric_field (PES_vector2d target, PES_FLOAT table_height)
{
 PES_EL_field electric_field;
 PES_vector2d position = get_pos();
 PES_FLOAT r = sqrt(pow(pit_dist(target, position),2) + pow(table_height,2));
 PES_vector2d diff = target - position;
 
 electric_field.field = diff*static_cast<PES_EM_TYPE>(charge/(pow(r,3)));  
 /// evaluating the jacobian
 electric_field.jacobian.row1.x = (pow(r,2) - 3*pow(diff.x,2));
 electric_field.jacobian.row1.y = -3* diff.x * diff.y;
 electric_field.jacobian.row2.x = (pow(r,2) - 3*pow(diff.y,2));
 electric_field.jacobian.row2.y = electric_field.jacobian.row1.y; 
 //renormalization
 electric_field.jacobian = electric_field.jacobian*static_cast<PES_FLOAT>(charge/pow(r,5));

 return electric_field;
};


////////////////////////////////////////////////
//                 SOLENOID                   //
PES_solenoid::PES_solenoid(const std::string data_string)
{
 std::istringstream data_stream(data_string);
 set_pos(data_stream);
 
 //default
 n_coils = current = radius = image_name = 0;

 if(data_stream.eof()) return; 
 data_stream >> n_coils;
 if(data_stream.eof()) return; 
 data_stream >> current;
 if(data_stream.eof()) return; 
 data_stream >> radius;
 if(data_stream.eof()) return; 
 data_stream >> image_name;
}


PES_MG_field
PES_solenoid::get_magnetic_field(PES_vector2d target)
{
 PES_MG_field mag_field;
 PES_vector2d position = get_pos();
 if (pit_dist(target, position) < radius)
 {
  mag_field.field = n_coils*current;
  mag_field.gradient = 0;
  return mag_field;
 }
 else return 0;
}
 

////////////////////////////////////////////////
//                    COIL                    //
PES_coil::PES_coil(const std::string data_string)
{
 std::istringstream data_stream(data_string);
 set_pos(data_stream);

 //default
 mag_dipole = image_name = 0;

 if(data_stream.eof()) return; 
 data_stream >> mag_dipole;
 if(data_stream.eof()) return; 
 data_stream >> image_name;
}

PES_MG_field
PES_coil::get_magnetic_field(PES_vector2d target, PES_FLOAT table_height)
{
 PES_MG_field mag_field;
 PES_vector2d position = get_pos();
 PES_FLOAT diff = sqrt( pow(pit_dist(target, position),2) + pow(table_height,2) );
 mag_field.field = 3*table_height*table_height*mag_dipole/pow(diff,5) - mag_dipole/pow(diff,3);

 return mag_field;
}


////////////////////////////////////////////////
//                    WIRE                    //
PES_wire::PES_wire(const std::string data_string)
{
 std::istringstream data_stream(data_string);
 set_pos(data_stream);
 
 //default
 linear_charge = image_name = 0;

 if(data_stream.eof()) return; 
 data_stream >> linear_charge;
 if(data_stream.eof()) return; 
 data_stream >> image_name;
}


PES_EL_field 
PES_wire::get_electric_field(PES_vector2d target)
{
 PES_EL_field el_field;
 PES_vector2d position = get_pos();

 el_field.field.x = linear_charge/pow(pit_dist(target,position),2)*(target-position).x;
 el_field.field.y = linear_charge/pow(pit_dist(target,position),2)*(target-position).y;
 
 return el_field;
}



////////////////////////////////////////////////
//                    DISK                    //
PES_disk::PES_disk(const std::string data_string)
{
 std::istringstream data_stream(data_string);
 //get pos and vel;
 set_pos(data_stream);
 set_vel(data_stream);

 //default
 charge = magdipole = goal_score = image_name = radius = 0;
 mass = 1;
 
 if(data_stream.eof()) return; 
 data_stream >> charge;
 if(data_stream.eof()) return; 
 data_stream >> magdipole;
 if(data_stream.eof()) return; 
 data_stream >> goal_score;
 if(data_stream.eof()) return; 
 data_stream >> image_name;
 if(data_stream.eof()) return; 
 data_stream >> radius;
 if(data_stream.eof()) return; 
 data_stream >> mass;

}



////////////////////////////////////////////////////////
//                        PES                         //
//         written by: Wronsky & Edi Edison           //
////////////////////////////////////////////////////////

/*
     EM STRUCTURES
Here we define matrices and fields, and we implement operators between them

usually included from PES_field_obj.h
*/

#ifndef EM_STRUCTS
#define EM_STRUCTS

#include "PES_types.h"

///////////////////// MATRIX //////////////////////
// 2x2 matrix with entries of EM_TYPE type
struct PES_EM_matrix2
{
 explicit PES_EM_matrix2 (PES_EM_vector2d input_row1 = 0, PES_EM_vector2d input_row2 = 0):
                 row1(input_row1), row2(input_row2) {};

 PES_EM_vector2d row1;
 PES_EM_vector2d row2;
};

//operators
PES_EM_matrix2 operator+ (const PES_EM_matrix2&, const PES_EM_matrix2&);
PES_EM_matrix2 operator- (const PES_EM_matrix2&, const PES_EM_matrix2&);
PES_EM_matrix2 operator* (const PES_EM_matrix2&, PES_FLOAT);
PES_EM_matrix2 operator/ (const PES_EM_matrix2&, PES_FLOAT);


///////////////////// FIELDS ////////////////////////
struct PES_EL_field
{

 //constructors
 PES_EL_field () {};
 PES_EL_field (PES_EM_vector2d input_field, PES_EM_matrix2 input_jacobian ): 
              field(input_field), jacobian(input_jacobian) {};

 PES_EM_vector2d field;
 PES_EM_matrix2 jacobian;

}; 

struct PES_MG_field
{
 PES_MG_field (PES_EM_TYPE input_field = 0, PES_EM_vector2d input_gradient = 0) :
                field(input_field), gradient(input_gradient) {};

 PES_EM_TYPE field;
 PES_EM_vector2d gradient;
};


//operators
PES_EL_field operator+ (const PES_EL_field&,const PES_EL_field&);
PES_EL_field operator- (const PES_EL_field&,const PES_EL_field&);
PES_EL_field operator* (const PES_EL_field&, PES_FLOAT);
PES_EL_field operator/ (const PES_EL_field&, PES_FLOAT);

PES_MG_field operator+ (const PES_MG_field&,const PES_MG_field&);
PES_MG_field operator- (const PES_MG_field&,const PES_MG_field&);
PES_MG_field operator* (const PES_MG_field&, PES_FLOAT);
PES_MG_field operator/ (const PES_MG_field&, PES_FLOAT);



#endif



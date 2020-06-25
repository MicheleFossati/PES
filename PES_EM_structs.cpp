////////////////////////////////////////////////////////
//                        PES                         //
//         written by: Wronsky & Edi Edison           //
////////////////////////////////////////////////////////


#include "PES_EM_structs.h"



////////////////   EM_MATRIX   /////////////////////////
PES_EM_matrix2 operator+ (const PES_EM_matrix2& m1,const PES_EM_matrix2& m2)
{
 PES_EM_matrix2 sum (m1.row1 + m2.row1, m1.row2 + m2.row2);
 return sum;
}

PES_EM_matrix2 operator- (const PES_EM_matrix2& m1,const PES_EM_matrix2& m2)
{
 PES_EM_matrix2 diff(m1.row1 - m2.row1, m1.row2 - m2.row2);
 return diff;
}

PES_EM_matrix2 operator*(const PES_EM_matrix2& m1, PES_FLOAT c)
{
 PES_EM_matrix2 product(m1.row1*c, m1.row2*c);
 return product;
}

PES_EM_matrix2 operator/ (const PES_EM_matrix2& m1, PES_FLOAT c)
{
return m1*(1/c);
}


///////////////   EL_FIELD    /////////////////////////
PES_EL_field operator+ (const PES_EL_field& f1,const PES_EL_field& f2)
{ 
 PES_EL_field sum (f1.field + f2.field, f1.jacobian + f2.jacobian);
 return sum;
}

PES_EL_field operator- (const PES_EL_field& f1,const PES_EL_field& f2)
{ 
 PES_EL_field diff(f1.field - f2.field, f1.jacobian - f2.jacobian);
 return diff;
}

PES_EL_field operator* (const PES_EL_field& f1, PES_FLOAT c)
{ 
 PES_EL_field product(f1.field*c, f1.jacobian*c);
 return product;
}

PES_EL_field operator/ (const PES_EL_field& f1, PES_FLOAT c)
{ 
 return f1*(1/c);
}


///////////////   MG_FIELD    /////////////////////////
PES_MG_field operator+ (const PES_MG_field& f1,const PES_MG_field& f2)
{ 
 PES_MG_field sum (f1.field + f2.field, f1.gradient + f2.gradient);
 return sum;
}

PES_MG_field operator- (const PES_MG_field& f1,const PES_MG_field& f2)
{ 
 PES_MG_field diff (f1.field - f2.field, f1.gradient - f2.gradient);
 return diff;
}

PES_MG_field operator * (const PES_MG_field& f, PES_FLOAT c)
{
 PES_MG_field product(f.field*c, f.gradient*c);
 return product;
}

PES_MG_field operator / (const PES_MG_field& f, PES_FLOAT c)
{
 return f*(1/c);
}



















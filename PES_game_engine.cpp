////////////////////////////////////////////////////////
//                        PES                         //
//         written by: Wronsky & Edi Edison           //
////////////////////////////////////////////////////////


#include "PES_game_engine.h"


///////////////////////////////////////////////////////////////////////
//                        CONSTRUCTORS                               //
PES_game_engine::PES_game_engine(PES_arena_ptr& a, PES_FLOAT dump) 
                 : dump_cost(dump), arena(a)
{  }


///////////////////////////////////////////////////////////////////////
//                          PROCESS									 //
bool
PES_game_engine::process(Uint32 t_interval)
{
 int nplayers = arena->get_n_obj(PLAYER_TYPE);
 int ndisks = arena->get_n_obj(DISK);
 


 //first check if anyone has scored
 for (int i = 0; i<ndisks; i++)
 {
  if (!score(i)) return false; 
 }
 
 //physics scify
 for (int i = 0; i<ndisks; i++)
  {

   //tactic: rigid collisions change disk instant velocity
   //wall collision also clips the disk inside the arena
   if (!wall_collision(i)) return false; 

   
   //collisions with players and other disks
   for (int j = 0; j<nplayers; j++)
   {
    if (!player_collision(i,j)) return false;
   }
   

   for (int j = i + 1; j<ndisks; j++)
   {
    if (!disk_collision(i,j)) return false;
   }
   
   //tactic: EM force changes both position and velocity
   EM_evolution(i, t_interval);
  }
 

return true;
}


//////////////////////////////////////////////////////////////////////////
//                       PRIVATE METHODS                                //
bool
PES_game_engine::EM_evolution(int disk, Uint32 dt)
{
  //checks
 if (!arena->check_n_obj(DISK, disk)) return false; //useless function calling? there are never enough checks
 if (dt == 0) return true;

 //useful objects
 PES_vector2d MG_force;
 PES_EM_TYPE charge = arena->get_obj_charge(DISK, disk);
 PES_EM_TYPE magdipole;// plz some1 implement this :(
 PES_vector2d vel = arena->get_obj_vel(DISK, disk);
 PES_vector2d pos = arena->get_obj_pos(DISK,disk);
 PES_MASS_TYPE mass = arena->get_disk_mass(disk);
 
 PES_FLOAT dt_f = dt/1000.;

 // RUNGE-KUTTA //////////////////////////////////////////////////////////////////////
 PES_vector2d k1, k2, k3, k4;
 PES_vector2d new_vel = vel;

 k1 = dt_f*get_force(pos, vel, charge, magdipole)/mass;
 new_vel += k1/6;

 k2 = dt_f*get_force(pos, vel + k1/2, charge, magdipole)/mass;
 new_vel += k2/3;

 k3 = dt_f*get_force(pos, vel + k2/2, charge, magdipole)/mass;
 new_vel += k3/3;

 k4 = dt_f*get_force(pos, vel + k3, charge, magdipole)/mass;
 new_vel += k4/6;

 
 PES_vector2d new_pos = pos + new_vel*(dt/1000.); //boh...

 //apply changes
 arena->set_obj_stat(DISK, disk, new_pos, new_vel); 
 
 return true;
}


PES_vector2d
PES_game_engine::get_force(PES_vector2d pos, PES_vector2d vel, 
                           PES_EM_TYPE charge, PES_EM_TYPE magdipole)
{
 //returns the force for a given velocity, position
 PES_vector2d EL_force, MG_force;

 //get EM fields
 PES_EL_field EL_field = arena->get_tot_EL_field(pos);
 PES_MG_field MG_field = arena->get_tot_MG_field(pos);

 //calculate Lorentz' force
 EL_force = EL_field.field*charge; 
 MG_force.x = charge*vel.y*MG_field.field;
 MG_force.y = -charge*vel.x*MG_field.field;

 return EL_force + MG_force - dump_cost*vel; //adding dump
}


///////////////////////////////////////////////////////////////////////
//                        GOALS                                      //

bool
PES_game_engine::score(int disk)  
{
 //checks
 if (!arena->check_n_obj(DISK, disk)) return false;

 int nrectgoals = arena->get_n_obj(RECT_GOAL);
 int nbumpers = arena->get_n_obj(BUMPER);

 for(int i = 0; i<nrectgoals; i++)
 { //collision with rect goals
  if(!rect_goal_collision(disk, i)) return false;
 }

 for(int i = 0; i<nbumpers; i++)
 { //collision with bumpers
  if(!bumper_collision(disk, i)) return false;
 }

 return true;
}

//rect goals
bool
PES_game_engine::rect_goal_collision(int disk, int goal)
{
 //checks
 if (!arena->check_n_obj(DISK, disk)) return false;
 if (!arena->check_n_obj(RECT_GOAL, goal)) return false;

 //get positions and dimensions 
 PES_FLOAT w = arena->get_obj_width(RECT_GOAL, goal);
 PES_FLOAT h = arena->get_obj_height(RECT_GOAL, goal);
 PES_FLOAT r = arena->get_obj_radius(DISK, disk);
 PES_vector2d pos_g = arena->get_obj_pos(RECT_GOAL, goal);
 PES_vector2d pos_d = arena->get_obj_pos(DISK, disk);

 //check if collision is occuring
 if( pos_d.x + r > pos_g.x - w/2  &&   //x min 
     pos_d.x - r < pos_g.x + w/2  &&   //x max
     pos_d.y + r > pos_g.y - h/2  &&   //y min
     pos_d.y - r < pos_g.y + h/2 )     //y max  ...I wish there was an analitic way...
 { //collision is occuring
  arena->scored(RECT_GOAL, goal, disk);   //correct player score
  //respawn
  arena->set_obj_stat(DISK, disk, arena->get_goal_respawn(goal), 0);
   PES_push_user_event(GOAL_RECT); 
  return true;
 }

 //collision hasn't occured
 return true;
}

//bumpers
bool   
PES_game_engine::bumper_collision(int disk, int bumper)
{
 //checks
 if (!arena->check_n_obj(DISK, disk)) return false;
 if (!arena->check_n_obj(BUMPER, bumper)) return false;
 

 //get positions and dimensions 
 PES_FLOAT   disk_r = arena->get_obj_radius(DISK, disk);
 PES_FLOAT bumper_r = arena->get_obj_radius(BUMPER, bumper);
 PES_vector2d pos_b = arena->get_obj_pos(BUMPER, bumper);
 PES_vector2d pos_d = arena->get_obj_pos(DISK, disk);
 PES_vector2d   vel = arena->get_obj_vel(DISK, disk);

 PES_vector2d dist = pos_b - pos_d;
 PES_FLOAT d = v2_modulus(dist);

 //check if collision is occuring
 if(d < disk_r + bumper_r  &&     //right pos
    vel*dist > 0 )                //right velocity
 { //collision is occuring
  arena->scored(BUMPER, bumper, disk);   //correct player score
  
  //actuate the collision
  PES_vector2d new_vel = vel - 2*arena->get_obj_boost(BUMPER, bumper)*(vel*dist)/pow(d,2)*dist;
  arena->set_obj_vel(DISK, disk, new_vel);
  PES_push_user_event(BUMPER_COLLISION);
  return true;
 }


 return true;
} 


///////////////////////////////////////////////////////////////////////
//                   PHYSICAL COLLISIONS                             //
bool
PES_game_engine::wall_collision(int disk)
{
 //checks
 if (!arena->check_n_obj(DISK, disk)) return false;

 PES_FLOAT width = arena->get_width();
 PES_FLOAT height = arena->get_height();
 PES_FLOAT r = arena->get_obj_radius(DISK, disk);
 if (r > width || r > height) return false;

 bool changed = false;
 PES_vector2d pos = arena->get_obj_pos(DISK, disk);
 PES_vector2d vel = arena->get_obj_vel(DISK, disk);

 
 //left to right
 if ((pos.x - r < 0 && vel.x < 0) || (pos.x + r > width && vel.x > 0)) 
 {
  vel.x = -vel.x;
  changed = true;
 }

 //up and down
 if ((pos.y -r < 0 && vel.y < 0) || (pos.y + r > height && vel.y > 0))
 {
  vel.y = -vel.y;
  changed = true;
 }

 if (!changed) return true;

 //applying the changes
 arena-> set_obj_vel(DISK, disk, vel);
 
//clipping
 PES_clip_obj(&pos, &vel, 0, arena->get_height(), 0, arena->get_width(),
              arena->get_obj_radius(DISK,disk));
 arena->set_obj_pos(DISK, disk, pos);

 //push WALL_COLLISION event
 PES_push_user_event(WALL_COLLISION);
 return true;
}


bool  
PES_game_engine::player_collision(int disk,int player)  
{
 //checks
 if (disk > arena->get_n_obj(DISK)) return false;
 if (player > arena->get_n_obj(PLAYER_TYPE)) return false;

//distance between the disks
 PES_vector2d d = arena->get_obj_pos(PLAYER_TYPE, player) 
                  - arena->get_obj_pos(DISK, disk);
 PES_FLOAT dist = v2_modulus(d);

 if (dist <= 0) 
  return true;

 //get radii
 PES_FLOAT r_d = arena->get_obj_radius(DISK, disk);
 PES_FLOAT r_p = arena->get_obj_radius(PLAYER_TYPE, player);
 //check if radii are acceptable
 if(r_d <= 0 || r_p <= 0) return false;

 //check if the collision is occuring
 if (dist > r_d + r_p) return true;

 //get vels 
 //relative velocity of the disk
 PES_vector2d p_vel = arena->get_obj_vel(PLAYER_TYPE, player);
 //working in the frame of reference of the player
 PES_vector2d vel_rel = arena->get_obj_vel(DISK, disk) - p_vel;
 PES_FLOAT scalar_pr = d*vel_rel;

 //check if the two obj's are actually colliding
 if(scalar_pr < 0) return true;
 
 PES_vector2d new_vel_rel = vel_rel - 2*scalar_pr/pow(dist,2)*d;
 
 //set the new velocity (translate back to field frame of reference)
 arena->set_obj_vel(DISK, disk, new_vel_rel + p_vel);

 //push PLAYER_COLLISION event
 PES_push_user_event(PLAYER_COLLISION);

 return true;
}


 

bool
PES_game_engine::disk_collision(int disk_a,int disk_b)  
{
 int n_disks = arena->get_n_obj(DISK);
 //checks
 if (disk_a > n_disks || disk_b > n_disks) return false;

 const PES_vector2d pos_a = arena->get_obj_pos(DISK,disk_a);
 const PES_vector2d pos_b = arena->get_obj_pos(DISK,disk_b);
 const PES_FLOAT r_a = arena->get_obj_radius(DISK,disk_a);
 const PES_FLOAT r_b = arena->get_obj_radius(DISK,disk_b); 
 const PES_FLOAT dist = pit_dist(pos_a,pos_b);
 
 //check if collision has occured
 if (dist > r_a + r_b) return true;
 if (dist <= 0) return true;

 //get vels and masses
 const PES_vector2d vel_a = arena->get_obj_vel(DISK, disk_a);
 const PES_vector2d vel_b = arena->get_obj_vel(DISK, disk_b);
 PES_MASS_TYPE mass_a = arena->get_disk_mass(disk_a);
 PES_MASS_TYPE mass_b = arena->get_disk_mass(disk_b);
 PES_MASS_TYPE M = mass_a + mass_b;

 //from now on, we assume that the disks are colliding
 PES_vector2d cm_vel = (vel_a*mass_a + vel_b*mass_b)/M;
 PES_vector2d cm_pos = (pos_a*mass_a + pos_b*mass_b)/M;

 //check if collision already occured on previous fames
 if ( (pos_a - cm_pos)*(vel_a - cm_vel) > 0) return true;



 //actual collision
 PES_vector2d new_vel_a, new_vel_b;

 PES_vector2d vel_a_rel = vel_a - cm_vel;
 PES_vector2d vel_b_rel = vel_b - cm_vel;

 PES_MASS_TYPE reduced_mass = 1/(1/mass_a + 1/mass_b);
 

 PES_vector2d x = -2*reduced_mass/pow(dist,2)*((pos_a - pos_b)*(vel_a_rel - vel_b_rel))*(pos_a - pos_b);  //impulse of the collision

 new_vel_a = vel_a + x/mass_a;
 new_vel_b = vel_b - x/mass_b;

 arena->set_obj_vel(DISK, disk_a, new_vel_a);
 arena->set_obj_vel(DISK, disk_b, new_vel_b);
 
 PES_push_user_event(DISK_COLLISION);

 return true;
}















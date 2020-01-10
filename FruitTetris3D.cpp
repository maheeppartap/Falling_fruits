
/*
	falling fruits 3d: assignment 2 for CMPT 361, Simon Fraser University 

	date edited: 03 Nov'2019

	Using angel.h supplied by Prof. Shermer.

	submitted by: Maheeppartap Singh

*/



#include "include/Angel.h"
#include <cstdlib>
#include <iostream>
#include <random>
#include <cstring>
#include <assert.h>
#include <math.h>
#include <unistd.h>

using namespace std;


//begin GLOBAL VARS
int timer;


vec2 CURR_TILE_GLOB[4]; 
vec2 CURR_TITLE_GLOB = vec2(5, 19); 



//change to alter screen size
GLfloat xsize = 720;
GLfloat ysize = 720;
GLfloat zsize = 720;



//add more shapes

vec2 I_shape_rotations_G[4][4] =
{ {vec2(0,0), vec2(-1,0), vec2(1,0), vec2(-2,0)},
{vec2(0,0), vec2(0,-1), vec2(0,1), vec2(0,-2)},
{vec2(-1,0), vec2(0,0), vec2(-2,0), vec2(1,0)},
{vec2(0,-1), vec2(0,0), vec2(0,-2), vec2(0,1)} };


vec2 R_shape_rotations_G[4][4] =
{ {vec2(0,0), vec2(-1,0), vec2(1,0), vec2(0,-1)},
{vec2(0,0), vec2(0,-1), vec2(0,1), vec2(1,0)},
{vec2(0,0), vec2(1,0), vec2(-1,0), vec2(0,1)},
{vec2(0,0), vec2(0,1), vec2(0,-1), vec2(-1,0)} };

vec2 L_shape_rotations_G[4][4] = 
	{{vec2(0,0), vec2(-1,0), vec2(1, 0), vec2(-1,-1)},
	{vec2(0,0), vec2(0,-1), vec2(0,1), vec2(1, -1)},     
	{vec2(0,0), vec2(1,0), vec2(-1, 0), vec2(1,  1)},  
	{vec2(0,0), vec2(0,1), vec2(0, -1), vec2(-1, 1)}};



vec2 S_shape_rotations_G[4][4] =
	{{vec2(0,-1), vec2(1,0), vec2(0,0), vec2(-1,-1)},
	{vec2(1,0), vec2(0,1), vec2(0,0), vec2(1,-1)},
	{vec2(0,0), vec2(-1,-1), vec2(0,-1), vec2(1,0)},
	{vec2(0,0), vec2(1,-1), vec2(1,0), vec2(0,1)}};




vec2 shapes[4][4][4] =
	{L_shape_rotations_G,
	R_shape_rotations_G,
	S_shape_rotations_G,
	I_shape_rotations_G};

enum orientation
{
	DOWN,
	RIGHT,
	UP,
	LEFT
};

enum tiletypes
{
	L,
	T,
	S,
	I
};

enum colors
{
	GREEN,
	BLUE,
	RED,
	ORANGE,
	YELLOW
};


int current_tile_type;
int current_tile_orientation;

vec4 inval_grey = vec4(0.5, 0.5, 0.5, 0.5); 
vec4 grey 	= 	vec4(0.8, 0.8, 0.8, 1.0);
vec4 green 	= 	vec4(0.0, 1.0, 0.0, 1.0); 
vec4 blue 	= 	vec4(0.0, 0.0, 1.0, 1.0); 
vec4 red 	=	vec4(1.0, 0.0, 0.0, 1.0); 
vec4 orange = 	vec4(1.0, 0.5, 0.0, 1.0);
vec4 yellow = 	vec4(1.0, 1.0, 0.0, 1.0); 
vec4 white  = 	vec4(1.0, 1.0, 1.0, 1.0);
vec4 black  = 	vec4(0.0, 0.0, 0.0, 0.0);


bool board[10][20];
bool isDropped = false;

int board_color__[10][20]; 

vec4 colour_in_board[1200];
vec4 other_col[24*6];
vec4 not_appli_arr[24*6];


GLuint v_p;
GLuint v_c;


GLuint shader_x;
GLuint shader_y;
GLuint shader_z;


vec4 colors[5] = { green, blue, red, orange, yellow };


GLuint vao_ID_[9]; 
GLuint vboID_[18]; 

bool move_dir(vec2 direction);
bool is_allowed(vec2 cur_tilepos);

enum { Yaxis = 0, Xaxis = 1, Zaxis = 2, NumAxes = 4 };
int ax_ = Xaxis;

GLfloat angle_[NumAxes] = { 0.0, 0.0, 0.0 }; 
GLuint theta;

GLfloat limit = 30;
GLfloat rate_ = 0.0;
GLfloat rate_x = -3;
GLfloat rate_z = -3;

const int vertice_BOARD = 36; 

vec4 COL_POINT[vertice_BOARD];
vec4 COL_COLOUR[vertice_BOARD];

GLfloat COL_A = 0.0;
GLfloat COL_B = 1.0;

vec4 VERT[8] = { 
    vec4( COL_A, COL_A, COL_B, 1.0 ),
    vec4( COL_A, COL_B, COL_B, 1.0 ),
    vec4( COL_B, COL_B, COL_B, 1.0 ),
    vec4( COL_B, COL_A, COL_B, 1.0 ),
    vec4( COL_A, COL_A, COL_A, 1.0 ),
    vec4( COL_A, COL_B, COL_A, 1.0 ),
    vec4( COL_B, COL_B, COL_A, 1.0 ),
    vec4( COL_B, COL_A, COL_A, 1.0 )
};

#define CUBE_base_h 33.0
#define CUBE_base_wid 66.0
#define ARM_UPPER_H 369.0
#define ARM_LOW_H 369.0
#define ARM_UPPER_wid  20.0
#define jo_bacha_h  40.0

enum {
    bottom,
    low_a,
    up_a,
    num_of_angles
};
GLint x = bottom - 1;	
GLfloat ang_gamma[num_of_angles] = {
90.0, 	
10.0, 	
-60.0 	
};


GLint my_new_angle = x; 

int Index = 0;




vec2 getTilePosition() {
	vec2 uttar_res;
	uttar_res.y += CUBE_base_h;
	uttar_res.x += ARM_LOW_H * -sin(M_PI/180.0 * ang_gamma[low_a]);
	uttar_res.y += ARM_LOW_H * cos(M_PI/180.0 * ang_gamma[low_a]);
	uttar_res.x += ARM_UPPER_H * -cos(M_PI/180.0 * (90.0 - ang_gamma[low_a] - ang_gamma[up_a]));
	uttar_res.y += ARM_UPPER_H *  sin(M_PI/180.0 * (90.0 - ang_gamma[low_a] - ang_gamma[up_a]));

	double val_doub;
	double ceil_v_doub;
	double floor_v_doub;
	for(int i = 0; i<2; i++){
		val_doub = uttar_res[i]/33.0;
		ceil_v_doub = ceil(val_doub);
		floor_v_doub = floor(val_doub);
		if((ceil_v_doub - val_doub) < (val_doub - floor_v_doub)) uttar_res[i] = ceil_v_doub;
		else uttar_res[i] = floor_v_doub;
	}
	return uttar_res;
}


void update_the_board()
{
	glBindBuffer(GL_ARRAY_BUFFER, vboID_[4]);

	for (int i = 0; i < 4; i++)
	{
		GLfloat x = CURR_TITLE_GLOB.x + CURR_TILE_GLOB[i].x;
		GLfloat y = CURR_TITLE_GLOB.y + CURR_TILE_GLOB[i].y;


		vec4 point_a = vec4(33.0 + (x * 33.0), 33.0 + (y * 33.0), 32, 1);
		vec4 point_b = vec4(33.0 + (x * 33.0), 66.0 + (y * 33.0), 32, 1);
		vec4 point_c = vec4(66.0 + (x * 33.0), 33.0 + (y * 33.0), 32, 1);
		vec4 point_d = vec4(66.0 + (x * 33.0), 66.0 + (y * 33.0), 32, 1);

		vec4 point_e = vec4(33.0 + (x * 33.0), 33.0 + (y * 33.0), 1, 1);
		vec4 point_f = vec4(33.0 + (x * 33.0), 66.0 + (y * 33.0), 1, 1);
		vec4 point_g = vec4(66.0 + (x * 33.0), 33.0 + (y * 33.0), 1, 1);
		vec4 point_h = vec4(66.0 + (x * 33.0), 66.0 + (y * 33.0), 1, 1);


		vec4 latest_pts[36] = {
								point_a, point_c, point_b,
								point_b, point_c, point_d,

								point_a, point_b, point_e,
								point_e, point_b, point_f,

								point_e, point_f, point_g,
								point_g, point_f, point_h,

								point_g, point_d, point_c,
								point_d, point_g, point_h,

								point_b, point_d, point_f,
								point_f, point_d, point_h,

								point_a, point_c, point_e,
								point_e, point_c, point_g
		};


		glBufferSubData(GL_ARRAY_BUFFER, i * 36 * sizeof(vec4), 36 * sizeof(vec4), latest_pts);
	}

	if (!is_allowed(getTilePosition()) && !isDropped) {
		glBindBuffer(GL_ARRAY_BUFFER, vboID_[5]);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(not_appli_arr), not_appli_arr);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	else if (!isDropped) {
		glBindBuffer(GL_ARRAY_BUFFER, vboID_[5]);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(other_col), other_col);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	glBindVertexArray(0);
}


void quad( int a, int b, int c, int d )
{
    COL_COLOUR[Index] = orange;
     COL_POINT[Index] = VERT[a];
      Index++;
    COL_COLOUR[Index] = orange; COL_POINT[Index] = VERT[b]; Index++;
    COL_COLOUR[Index] = orange; COL_POINT[Index] = VERT[c]; Index++;
    COL_COLOUR[Index] = orange; COL_POINT[Index] = VERT[a]; Index++;
    COL_COLOUR[Index] = orange; COL_POINT[Index] = VERT[c]; Index++;
    COL_COLOUR[Index] = orange; COL_POINT[Index] = VERT[d]; Index++;
}

void cube( void )
{
	Index = 0;
    quad( 1, 0, 3, 2 );
    quad( 2, 3, 7, 6 );
    quad( 3, 0, 4, 7 );
    quad( 6, 5, 1, 2 );
    quad( 4, 5, 6, 7 );
    quad( 5, 4, 0, 1 );
}


class mat_stack_struct {
	int ind;
	int size_stack;
	mat4* actual_stack_hidden;

public:
	mat_stack_struct( int numMatrices = 32):ind(0), size_stack(numMatrices)
	{ actual_stack_hidden = new mat4[numMatrices]; }

	~mat_stack_struct()
	{ delete[]actual_stack_hidden; }

	mat4& push(const mat4& m ){
		assert( ind +1 < size_stack);
		actual_stack_hidden[ind++] = m;
	}

	mat4& pop( void ) {
		assert( ind - 1 >= 0);
		ind--;
		return actual_stack_hidden[ind];
	}
};

mat_stack_struct mystack;
mat4 model_view_matrice, camera_view_matrice, projection_view_matrice;
GLuint model_view_int;
mat4 Id = mat4(1.0);

//found this on the internet, apparently works like a charm
////////////////////////////////////////////////////////////////////////////////////////////////////
//todo: check this
int personalized_rand(int bound){
	random_device rd;
	mt19937 gen(rd());	//no idea what mt19937 is
	uniform_int_distribution<> dist(0,bound);
	int result = dist(gen);
	return result;
}


void initGrid()
{

	vec4 gridpoints[128];
	vec4 gridcolours[128];

	for (int i = 0; i < 11; i++) {
		gridpoints[2 * i] = vec4((33.0 + (33.0 * i)), 33.0, 33.0, 1);
		gridpoints[2 * i + 1] = vec4((33.0 + (33.0 * i)), 693.0, 33.0, 1);

		gridpoints[22 + 2 * i] = vec4((33.0 + (33.0 * i)), 33.0, 0, 1);
		gridpoints[22 + 2 * i + 1] = vec4((33.0 + (33.0 * i)), 693.0, 0, 1);

	}
	for (int i = 0; i < 21; i++) {
		gridpoints[44 + 2 * i] = vec4(33.0, (33.0 + (33.0 * i)), 33.0, 1);
		gridpoints[44 + 2 * i + 1] = vec4(363.0, (33.0 + (33.0 * i)), 33.0, 1);

		gridpoints[86 + 2 * i] = vec4(35.0, (33.0 + (33.0 * i)), 0, 1);
		gridpoints[86 + 2 * i + 1] = vec4(365.0, (33.0 + (33.0 * i)), 0, 1);
	}
	for (int i = 0; i < 128; i++)
		gridcolours[i] = white;


	glBindVertexArray(vao_ID_[0]);
	glGenBuffers(2, vboID_);


	glBindBuffer(GL_ARRAY_BUFFER, vboID_[0]);
	glBufferData(GL_ARRAY_BUFFER, 128 * sizeof(vec4), gridpoints, GL_STATIC_DRAW);
	glVertexAttribPointer(v_p, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(v_p);


	glBindBuffer(GL_ARRAY_BUFFER, vboID_[1]);
	glBufferData(GL_ARRAY_BUFFER, 128 * sizeof(vec4), gridcolours, GL_STATIC_DRAW);
	glVertexAttribPointer(v_c, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(v_c);
}



void newtile()
{
	CURR_TITLE_GLOB = vec2(5 , 19); 
	
	int tile_type = personalized_rand(3);
	current_tile_orientation = DOWN;
	current_tile_type = tile_type;
	int current_color = 0;

	for (int i = 0; i < 4; i++)
		switch(tile_type) {
			case 0:
				CURR_TILE_GLOB[i] = L_shape_rotations_G[current_tile_orientation][i]; 
				break;
			case 1:
				CURR_TILE_GLOB[i] = R_shape_rotations_G[current_tile_orientation][i]; 
				break;
			case 2:
				CURR_TILE_GLOB[i] = S_shape_rotations_G[current_tile_orientation][i]; 
				break;
			case 3:
				CURR_TILE_GLOB[i] = I_shape_rotations_G[current_tile_orientation][i]; 
				break;
			default:
				break;
		}
	update_the_board(); 

	
	for (int i = 0; i < 144; i++){
		if((i%36)==0){
			current_color = personalized_rand(4);
		}

		other_col[i] = colors[current_color]; 
	}
	glBindBuffer(GL_ARRAY_BUFFER, vboID_[5]); 
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(other_col), other_col); 
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);



	if(board[5][19] == true || board[4][19] == true || board[6][19] == true) {
		printf("Game over");
		sleep(1);
		exit(EXIT_SUCCESS);
	}
}



void initBoard()
{
	
	GLfloat z_POS_1, z_POS_2;
	
	z_POS_1 = 1.0;
	z_POS_2 = 32.0;


	for (int i = 0; i < 1200; i++)
		colour_in_board[i] = black;

	//MAYBE GET RID????????
	
	vec4 points_in_board[1200];
	vec4 reversed_points_in_board[1200];
	vec4 boardpoints_right[1200];
	vec4 boardpoints_left[1200];
	vec4 boardpoints_bottom[1200];
	vec4 boardpoints_top[1200];

	for (int i = 0; i < 20; i++){
		for (int j = 0; j < 10; j++)
		{	

			vec4 p1 = vec4(33.0 + (j * 33.0), 33.0 + (i * 33.0), z_POS_1, 1);
			vec4 p2 = vec4(33.0 + (j * 33.0), 66.0 + (i * 33.0), z_POS_1, 1);
			vec4 p3 = vec4(66.0 + (j * 33.0), 33.0 + (i * 33.0), z_POS_1, 1);
			vec4 p4 = vec4(66.0 + (j * 33.0), 66.0 + (i * 33.0), z_POS_1, 1);
			
			points_in_board[6*(10*i + j)    ] = p1;
			points_in_board[6*(10*i + j) + 1] = p2;
			points_in_board[6*(10*i + j) + 2] = p3;
			points_in_board[6*(10*i + j) + 3] = p2;
			points_in_board[6*(10*i + j) + 4] = p3;
			points_in_board[6*(10*i + j) + 5] = p4;

			p1 = vec4(33.0 + (j * 33.0), 33.0 + (i * 33.0), z_POS_2, 1);
			p2 = vec4(33.0 + (j * 33.0), 66.0 + (i * 33.0), z_POS_2, 1);
			p3 = vec4(66.0 + (j * 33.0), 33.0 + (i * 33.0), z_POS_2, 1);
			p4 = vec4(66.0 + (j * 33.0), 66.0 + (i * 33.0), z_POS_2, 1);		
			
			reversed_points_in_board[6*(10*i + j)    ] = p1;
			reversed_points_in_board[6*(10*i + j) + 1] = p2;
			reversed_points_in_board[6*(10*i + j) + 2] = p3;
			reversed_points_in_board[6*(10*i + j) + 3] = p3;
			reversed_points_in_board[6*(10*i + j) + 4] = p4;
			reversed_points_in_board[6*(10*i + j) + 5] = p2;

			p1 = vec4(66.0 + (j * 33.0), 33.0 + (i * 33.0), z_POS_1, 1);
			p2 = vec4(66.0 + (j * 33.0), 66.0 + (i * 33.0), z_POS_1, 1);
			p3 = vec4(66.0 + (j * 33.0), 33.0 + (i * 33.0), z_POS_2, 1);
			p4 = vec4(66.0 + (j * 33.0), 66.0 + (i * 33.0), z_POS_2, 1);		
		
			boardpoints_right[6*(10*i + j)    ] = p1;
			boardpoints_right[6*(10*i + j) + 1] = p3;
			boardpoints_right[6*(10*i + j) + 2] = p2;
			boardpoints_right[6*(10*i + j) + 3] = p2;
			boardpoints_right[6*(10*i + j) + 4] = p3;
			boardpoints_right[6*(10*i + j) + 5] = p4;

			p1 = vec4(33.0 + (j * 33.0), 33.0 + (i * 33.0), z_POS_1, 1);
			p2 = vec4(33.0 + (j * 33.0), 66.0 + (i * 33.0), z_POS_1, 1);
			p3 = vec4(33.0 + (j * 33.0), 33.0 + (i * 33.0), z_POS_2, 1);
			p4 = vec4(33.0 + (j * 33.0), 66.0 + (i * 33.0), z_POS_2, 1);		
		
			boardpoints_left[6*(10*i + j)    ] = p1;
			boardpoints_left[6*(10*i + j) + 1] = p3;
			boardpoints_left[6*(10*i + j) + 2] = p2;
			boardpoints_left[6*(10*i + j) + 3] = p2;
			boardpoints_left[6*(10*i + j) + 4] = p4;
			boardpoints_left[6*(10*i + j) + 5] = p3;

			p1 = vec4(33.0 + (j * 33.0), 33.0 + (i * 33.0), z_POS_1, 1);
			p2 = vec4(33.0 + (j * 33.0), 33.0 + (i * 33.0), z_POS_2, 1);
			p3 = vec4(66.0 + (j * 33.0), 33.0 + (i * 33.0), z_POS_1, 1);
			p4 = vec4(66.0 + (j * 33.0), 33.0 + (i * 33.0), z_POS_2, 1);		
			// bottom side
			boardpoints_bottom[6*(10*i + j)    ] = p1;
			boardpoints_bottom[6*(10*i + j) + 1] = p2;
			boardpoints_bottom[6*(10*i + j) + 2] = p3;
			boardpoints_bottom[6*(10*i + j) + 3] = p2;
			boardpoints_bottom[6*(10*i + j) + 4] = p3;
			boardpoints_bottom[6*(10*i + j) + 5] = p4;

			p1 = vec4(33.0 + (j * 33.0), 66.0 + (i * 33.0), z_POS_1, 1);
			p2 = vec4(33.0 + (j * 33.0), 66.0 + (i * 33.0), z_POS_2, 1);
			p3 = vec4(66.0 + (j * 33.0), 66.0 + (i * 33.0), z_POS_1, 1);
			p4 = vec4(66.0 + (j * 33.0), 66.0 + (i * 33.0), z_POS_2, 1);		
			
			boardpoints_top[6*(10*i + j)    ] = p1;
			boardpoints_top[6*(10*i + j) + 1] = p3;
			boardpoints_top[6*(10*i + j) + 2] = p2;
			boardpoints_top[6*(10*i + j) + 3] = p2;
			boardpoints_top[6*(10*i + j) + 4] = p3;
			boardpoints_top[6*(10*i + j) + 5] = p4;

		}
	}


	for (int i = 0; i < 10; i++)
		for (int j = 0; j < 20; j++){
			board[i][j] = false;
			board_color__[i][j] = -1; 
		}

	glBindVertexArray(vao_ID_[1]);
	glGenBuffers(2, &vboID_[2]);

	glBindBuffer(GL_ARRAY_BUFFER, vboID_[2]);
	glBufferData(GL_ARRAY_BUFFER, 1200*sizeof(vec4), points_in_board, GL_STATIC_DRAW);
	glVertexAttribPointer(v_p, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(v_p);

	glBindBuffer(GL_ARRAY_BUFFER, vboID_[3]);
	glBufferData(GL_ARRAY_BUFFER, 1200*sizeof(vec4), colour_in_board, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(v_c, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(v_c);

	glBindVertexArray(vao_ID_[3]);
	glGenBuffers(2, &vboID_[6]);

	glBindBuffer(GL_ARRAY_BUFFER, vboID_[6]);
	glBufferData(GL_ARRAY_BUFFER, 1200*sizeof(vec4), reversed_points_in_board, GL_STATIC_DRAW);
	glVertexAttribPointer(v_p, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(v_p);

	glBindBuffer(GL_ARRAY_BUFFER, vboID_[3]);
	glBufferData(GL_ARRAY_BUFFER, 1200*sizeof(vec4), colour_in_board, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(v_c, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(v_c);

	glBindVertexArray(vao_ID_[4]);
	glGenBuffers(2, &vboID_[8]);

	glBindBuffer(GL_ARRAY_BUFFER, vboID_[8]);
	glBufferData(GL_ARRAY_BUFFER, 1200*sizeof(vec4), boardpoints_right, GL_STATIC_DRAW);
	glVertexAttribPointer(v_p, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(v_p);

	glBindBuffer(GL_ARRAY_BUFFER, vboID_[3]);
	glBufferData(GL_ARRAY_BUFFER, 1200*sizeof(vec4), colour_in_board, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(v_c, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(v_c);

	glBindVertexArray(vao_ID_[5]);
	glGenBuffers(2, &vboID_[10]);

	
	glBindBuffer(GL_ARRAY_BUFFER, vboID_[10]);
	glBufferData(GL_ARRAY_BUFFER, 1200*sizeof(vec4), boardpoints_left, GL_STATIC_DRAW);
	glVertexAttribPointer(v_p, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(v_p);

	glBindBuffer(GL_ARRAY_BUFFER, vboID_[3]);
	glBufferData(GL_ARRAY_BUFFER, 1200*sizeof(vec4), colour_in_board, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(v_c, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(v_c);

	glBindVertexArray(vao_ID_[6]);
	glGenBuffers(2, &vboID_[12]);

	
	glBindBuffer(GL_ARRAY_BUFFER, vboID_[12]);
	glBufferData(GL_ARRAY_BUFFER, 1200*sizeof(vec4), boardpoints_bottom, GL_STATIC_DRAW);
	glVertexAttribPointer(v_p, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(v_p);

	
	glBindBuffer(GL_ARRAY_BUFFER, vboID_[3]);
	glBufferData(GL_ARRAY_BUFFER, 1200*sizeof(vec4), colour_in_board, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(v_c, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(v_c);

	
	glBindVertexArray(vao_ID_[7]);
	glGenBuffers(2, &vboID_[14]);


	glBindBuffer(GL_ARRAY_BUFFER, vboID_[14]);
	glBufferData(GL_ARRAY_BUFFER, 1200*sizeof(vec4), boardpoints_top, GL_STATIC_DRAW);
	glVertexAttribPointer(v_p, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(v_p);

	
	glBindBuffer(GL_ARRAY_BUFFER, vboID_[3]);
	glBufferData(GL_ARRAY_BUFFER, 1200*sizeof(vec4), colour_in_board, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(v_c, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(v_c);

}


void in_tile()
{
	glBindVertexArray(vao_ID_[2]);
	glGenBuffers(2, &vboID_[4]);

	
	glBindBuffer(GL_ARRAY_BUFFER, vboID_[4]);
	glBufferData(GL_ARRAY_BUFFER, 4*36*sizeof(vec4), NULL, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(v_p, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(v_p);

	
	glBindBuffer(GL_ARRAY_BUFFER, vboID_[5]);
	glBufferData(GL_ARRAY_BUFFER, 4*36*sizeof(vec4), NULL, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(v_c, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(v_c);
}

void in_cube()
{
	cube();

	glBindVertexArray(vao_ID_[8]);
	glGenBuffers(2, &vboID_[16]);

	glBindBuffer(GL_ARRAY_BUFFER, vboID_[16]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(COL_POINT), COL_POINT, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(v_p, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(v_p);

	glBindBuffer(GL_ARRAY_BUFFER, vboID_[17]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(COL_COLOUR), COL_COLOUR, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(v_c, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(v_c);
}

void init()
{
	
	//change this if possible
	
	GLuint program = InitShader("vshader81.glsl", "fshader81.glsl");
	glUseProgram(program);

	
	v_p = glGetAttribLocation(program, "vPosition");
	v_c = glGetAttribLocation(program, "vColor");

	glGenVertexArrays(9, &vao_ID_[0]);

	for(int i = 0; i<144; i++){
		not_appli_arr[i] = inval_grey;
	}

	in_tile();
	initGrid();
	initBoard();
	in_cube();

	isDropped = false;
	newtile(); 


	model_view_int = glGetUniformLocation( program, "ModelView" );
	glEnable( GL_DEPTH_TEST );

	camera_view_matrice = LookAt(vec3(0.0, 30.0, 30.0),	vec3(0.0, 10.0, 0.0), vec3(0.0, 1.0, 0.0));

	glBindVertexArray(0);
	glClearColor(0.6, 0.4, 0.4, 0);
}



bool is_rot_poss(){
	int x_pos = CURR_TITLE_GLOB.x;
	int y_pos = CURR_TITLE_GLOB.y;

	vec2 all_cell_pos[4] = shapes[current_tile_type][current_tile_orientation]; 
	for(int j = 0; j<4; ++j){ 
		all_cell_pos[j].x += x_pos;
		all_cell_pos[j].y += y_pos;
	}

	for(int i = 0; i<4; ++i){
		if(board[(int)all_cell_pos[i].x][(int)all_cell_pos[i].y]==true) return false;
	}

	bool move_attempt;


	//added sup for floor
	switch(current_tile_type) {
		case L:	
			if(x_pos == 0) {
				move_attempt = move_dir(vec2(1,0));
				return move_attempt;
			}
			else if(x_pos == 9) {
				move_attempt = move_dir(vec2(-1,0));
				return move_attempt;
			}
			else if(y_pos == 0) { 
				return false;
			}
			else if(y_pos==19) {
				move_attempt = move_dir(vec2(0,-1));
				return move_attempt;
			}	
			break;
		case T:
			if(x_pos == 0) {
				move_attempt = move_dir(vec2(1,0));
				return move_attempt;
			}
			else if(x_pos == 9) {
				move_attempt = move_dir(vec2(-1,0));
				return move_attempt;
			}
			else if(y_pos == 0) { 
				return false;
			}
			else if(y_pos==19) {
				move_attempt = move_dir(vec2(0,-1));
				return move_attempt;
			}
			break;
		case S:
			if(x_pos == 0) {
				move_attempt = move_dir(vec2(1,0));
				return move_attempt;
			}
			else if(y_pos==19) {
				move_attempt = move_dir(vec2(0,-1));
				return move_attempt;
			}
			break;
		case I:
			if(x_pos == 0) {
				move_attempt = move_dir(vec2(2,0));
				return move_attempt;
			}
			else if(x_pos == 1){
				move_attempt = move_dir(vec2(1,0));
				return move_attempt;
			}
			else if(x_pos == 9) {
				move_attempt = move_dir(vec2(-1,0));
				return move_attempt;
			}
			else if(y_pos == 0 || y_pos == 1) { 
				return false;
			}
			else if(y_pos==19) {
				move_attempt = move_dir(vec2(0,-1));
				return move_attempt;
			}
			break;
		default:
			break;
	}
	return true;
}


void change_rot(){
	for (int i = 0; i < 4; i++)
		switch(current_tile_type) {
			case L:
				CURR_TILE_GLOB[i] = L_shape_rotations_G[current_tile_orientation][i]; 
				break;
			case T:
				CURR_TILE_GLOB[i] = R_shape_rotations_G[current_tile_orientation][i]; 
				break;
			case S:
				CURR_TILE_GLOB[i] = S_shape_rotations_G[current_tile_orientation][i]; 
				break;
			case I:
				CURR_TILE_GLOB[i] = I_shape_rotations_G[current_tile_orientation][i]; 
				break;
			default:
			break;
		}
}

void rotate()
{      
	switch(current_tile_orientation){
		case DOWN:
			current_tile_orientation = RIGHT;
			change_rot();
			if(!is_rot_poss()){
				current_tile_orientation = DOWN;
				change_rot();
			}
			break;
		case RIGHT:
			current_tile_orientation = UP;
			change_rot();
			if(!is_rot_poss()){
				current_tile_orientation = RIGHT;
				change_rot();
			}
			break;
		case UP:
			current_tile_orientation = LEFT;
			change_rot();
			if(!is_rot_poss()){
				current_tile_orientation = UP;
				change_rot();
			}
			break;
		case LEFT:
			current_tile_orientation = DOWN;
			change_rot();
			if(!is_rot_poss()){
				current_tile_orientation = LEFT;
				change_rot();
			}
			break;
		default:
			break;
	}
	update_the_board();
}

//TODO: improve this if you have time

bool checking_for_match(){
	int col_lower, col_higher, row_lower, row_higher, e; 

	col_lower = 0; 
	col_higher = 0; 
	e = board_color__[0][0]; 

	int intsize = sizeof(board_color__[0][0]);

	for(int i = 0; i<10; ++i){ //check for column matches
		col_lower = 0; col_higher = 0;
		e = board_color__[i][0];

		for(int j = 0; j<20; ++j){
			if(board_color__[i][j]==-1 && !(col_higher-col_lower>2)){
				e = -1; col_lower = col_higher;
				col_lower++; 
				col_higher++;
			}
			else if(board_color__[i][j]!=e && !(col_higher-col_lower>2)){
				e = board_color__[i][j]; col_lower = col_higher;
				col_higher++;
			}
			else if(board_color__[i][j]!=e && (col_higher-col_lower>2)){				
				
				memmove(board[i]+col_lower, board[i]+col_higher, 20-1-col_higher);
				memmove(board_color__[i]+col_lower, board_color__[i]+col_higher, 20*intsize-intsize-intsize*col_higher); //possibility of integer size difference between systems
				board[i][19]=false;
				board_color__[i][19]=-1;

				for(int k = 0; k<(19-col_higher); ++k){					
					memmove(colour_in_board+((k+col_lower)*60)+(i*6), colour_in_board+((k+col_higher)*60)+(i*6), 96);
				}
				colour_in_board[19*60+(i*6)] = black;
				checking_for_match();
				return true;
				
				e = board_color__[i][j]; col_higher=col_lower; col_higher++;
			}
			else if(board_color__[i][j]==e){
				col_higher++;
			}
		}
	}

	//cpuld be issues with size difference. If buggy, 99% this....
	for(int j = 0; j<20; ++j){ 
		row_lower = 0; row_higher = 0;
		e = board_color__[0][j];

		for(int i = 0; i<10; ++i){
			if(board_color__[i][j]==-1 && !(row_higher-row_lower>2)){
				e = -1; row_lower = row_higher; row_lower++; row_higher++;
			}
			else if(board_color__[i][j]!=e && !(row_higher-row_lower>2)){
				e = board_color__[i][j]; row_lower = row_higher; row_higher++;
			}
			else if(board_color__[i][j]!=e && (row_higher-row_lower>2)){ 
				for(int h = 0; h<(row_higher-row_lower); ++h){
					memmove(board[row_lower+h]+j, board[row_lower+h]+j+1, 20-j-1);
					memmove(board_color__[row_lower+h]+j, board_color__[row_lower+h]+j+1, 20*intsize-intsize-intsize*j); 
					board[row_lower+h][19]=false;
					board_color__[row_lower+h][19]=-1;
				}

				for(int k = 0; k<(19-j); ++k){					
					memmove(colour_in_board+((k+j)*60)+(row_lower*6), colour_in_board+((k+1+j)*60)+(row_lower*6), 96*(row_higher-row_lower));
				}
				for(int h = row_lower; h<row_higher; ++h){
					colour_in_board[19*60+(h*6)] = black;
				}
				checking_for_match();

				return true;

			}
			else if(board_color__[i][j]==e){
				row_higher++;
			}	
		}

	}

	glBindBuffer(GL_ARRAY_BUFFER, vboID_[3]);
	glBufferData(GL_ARRAY_BUFFER, 1200*sizeof(vec4), colour_in_board, GL_DYNAMIC_DRAW);

	return true;	
}

void checkfullrow(int row) {
	for(int k = 0; k<10; ++k){
		if(board[k][row]==false) {
			return;
		}
	}

	int intsize = sizeof(board_color__[0][0]);

	for(int i = 0; i<10; ++i){ 
		memmove(board[i]+row, board[i]+row+1, 20-1-row);
		memmove(board_color__[i]+row, board_color__[i]+row+1, 20*intsize-intsize-intsize*row); 
		board[i][19]=false;
		board_color__[i][19]=-1;
	}
	
	//this is very sketchy...
	memmove(colour_in_board+(row*60), colour_in_board+((row+1)*60), (18240)-((row)*60*16)); 
	for(int j = 1140; j< 1200; ++j)
		colour_in_board[j] = black;
	

	glBindBuffer(GL_ARRAY_BUFFER, vboID_[3]);
	glBufferData(GL_ARRAY_BUFFER, 1200*sizeof(vec4), colour_in_board, GL_DYNAMIC_DRAW);

	checking_for_match();
}


void settile()
{

	int low = 20; int high = -1;
	int x_pos = CURR_TITLE_GLOB.x;
	int y_pos = CURR_TITLE_GLOB.y;

	vec2 all_cell_pos[4] = shapes[current_tile_type][current_tile_orientation]; 

	for(int j = 0; j<4; ++j){
		all_cell_pos[j].x += x_pos;
		all_cell_pos[j].y += y_pos;
		if((int)all_cell_pos[j].y > high) high = (int)all_cell_pos[j].y;
		if((int)all_cell_pos[j].y < low) low = (int)all_cell_pos[j].y;
	}

	//improve this 
	for(int i = 0; i < 4; ++i){ 
		for(int k = 0; k < 6; ++k){ 
			colour_in_board[(60*(int)all_cell_pos[i].y + 6*(int)all_cell_pos[i].x) + k] = {other_col[k+(i*36)][0], other_col[k+(i*36)][1], other_col[k+(i*36)][2], other_col[k+(i*36)][3]};
		}

		if((other_col[i*36][0] == 0.0f) && (other_col[i*36][1] == 1.0f) && (other_col[i*36][2] == 0.0f) && (other_col[i*36][3] == 1.0f)){ 
			board_color__ [(int)all_cell_pos[i].x][(int)all_cell_pos[i].y] = GREEN;
		}
		else if((other_col[i*36][0] == 0.0f) && (other_col[i*36][1] == 0.0f) && (other_col[i*36][2] == 1.0f) && (other_col[i*36][3] == 1.0f)){ 
			board_color__ [(int)all_cell_pos[i].x][(int)all_cell_pos[i].y] = BLUE;
		}
		else if ((other_col[i * 36][0] == 1.0f) && (other_col[i * 36][1] == 1.0f) && (other_col[i * 36][2] == 0.0f) && (other_col[i * 36][3] == 1.0f)) { 
			board_color__[(int)all_cell_pos[i].x][(int)all_cell_pos[i].y] = YELLOW;
		}
		else if((other_col[i*36][0] == 1.0f) && (other_col[i*36][1] == 0.0f) && (other_col[i*36][2] == 0.0f) && (other_col[i*36][3] == 1.0f)){
			board_color__ [(int)all_cell_pos[i].x][(int)all_cell_pos[i].y] = RED;
		}
		else if((other_col[i*36][0] == 1.0f) && (other_col[i*36][1] == 0.5f) && (other_col[i*36][2] == 0.0f) && (other_col[i*36][3] == 1.0f)){ 
			board_color__ [(int)all_cell_pos[i].x][(int)all_cell_pos[i].y] = ORANGE;
		}
		



		board[(int)all_cell_pos[i].x][(int)all_cell_pos[i].y] = true;

	}

	glBindBuffer(GL_ARRAY_BUFFER, vboID_[3]);
	glBufferData(GL_ARRAY_BUFFER, 1200*sizeof(vec4), colour_in_board, GL_DYNAMIC_DRAW);

	
	for(int l = high; l >=low; --l){ 
		checkfullrow(l);
	}

	checking_for_match();

	newtile();
	isDropped = false;
}



bool move_dir(vec2 direction)
{
	int left_border, right_border, bottom_border;
	left_border = 0; right_border = 0; bottom_border = 0;

	switch(current_tile_type) {
		case L: 
			switch(current_tile_orientation) {
				case DOWN:
					left_border = 1;
					right_border = 8;
					bottom_border = 1;
					break;
				case RIGHT:
					left_border = 0; 
					right_border = 8; 
					bottom_border = 1;
					break;
				case UP:
					left_border = 1; 
					right_border = 8;
					bottom_border = 0;
					break;
				case LEFT:
					left_border = 1;
					right_border = 9; 
					bottom_border = 1;
					break;
				default:
					break;
			}
			break;
		case T: 
			switch(current_tile_orientation) {
				case DOWN:
					left_border = 1;
					right_border = 8; 
					bottom_border = 1;
					break;
				case RIGHT:
					left_border = 0; 
					right_border = 8; 
					bottom_border = 1;
					break;
				case UP:
					left_border = 1;
					right_border = 8;
					bottom_border = 0;
					break;
				case LEFT:
					left_border = 1;
					right_border = 9; 
					bottom_border = 1;
					break;
				default:
					break;
			}
			break;
		case S: 
			switch(current_tile_orientation) {
				case DOWN:
					left_border = 1;
					right_border = 8; 
					bottom_border = 1;
					break;
				case RIGHT:
					left_border = 0;
					right_border = 8; 
					bottom_border = 1;
					break;
				case UP:
					left_border = 1;
					right_border = 8;
					bottom_border = 1;
					break;
				case LEFT:
					left_border = 0;
					right_border = 8;
					bottom_border = 1;
					break;
				default:
					break;
			}
			break;
		case I: 
			switch(current_tile_orientation) {
				case DOWN:
					left_border = 2;
					right_border = 8;
					bottom_border = 0;
					break;
				case RIGHT:
					left_border = 0; 
					right_border = 9;
					bottom_border = 2;
					break;
				case UP:
					left_border = 2;
					right_border = 8;
					bottom_border = 0;
					break;
				case LEFT:
					left_border = 0;
					right_border = 9;
					bottom_border = 2;
					break;
				default:
					break;
			}
			break;
		default:
			break;
	}

	int x_pos = CURR_TITLE_GLOB.x;
	int y_pos = CURR_TITLE_GLOB.y;

	vec2 all_cell_pos[4] = shapes[current_tile_type][current_tile_orientation]; 


	for(int j = 0; j<4; ++j){ 
		all_cell_pos[j].x += x_pos;
		all_cell_pos[j].y += y_pos;
	}

	for(int k = 0; k<4; ++k){
		if((board[(int)all_cell_pos[k].x][(int)all_cell_pos[k].y-1]==true) && (direction.y!=0)){
			settile();
			return false;
		} 
	}

	if(CURR_TITLE_GLOB.y==bottom_border && direction.y!=0) {
		settile();
		return false;
	}

	if(CURR_TITLE_GLOB.y+direction.y >= bottom_border) CURR_TITLE_GLOB.y += direction.y;
	if((CURR_TITLE_GLOB.x+direction.x >= left_border) && 
		(CURR_TITLE_GLOB.x+direction.x <= right_border)){ 
		for(int k = 0; k<4; ++k){
			if(board[(int)all_cell_pos[k].x + (int)direction.x][(int)all_cell_pos[k].y]==true){
				return false;
			}
		}
		CURR_TITLE_GLOB.x += direction.x;
	}
	return true;
}


//here we go again
void here_we_go_again()
{
	init();
}


void base(){
	mystack.push(model_view_matrice);
	glBindVertexArray(vao_ID_[8]);
	mat4 instance = (Translate(-33.0, 0.0, 33.0)*Scale(99.0, 33.0, 99.0));
	glUniformMatrix4fv(model_view_int, 1, GL_TRUE, model_view_matrice * instance);
	glColor3i(100,0,0);
	glDrawArrays(GL_TRIANGLES, 0, vertice_BOARD);
	model_view_matrice = mystack.pop();
}

void lower(){
	mystack.push(model_view_matrice);
	glBindVertexArray(vao_ID_[8]);
	mat4 instance = (
		Translate(0.0, 0.0, 38.0)
		*Scale(jo_bacha_h, ARM_LOW_H, jo_bacha_h)
		);
	glUniformMatrix4fv(model_view_int, 1, GL_TRUE, model_view_matrice * instance);
	glDrawArrays(GL_TRIANGLES, 0, vertice_BOARD);
	model_view_matrice = mystack.pop();
}

void upper(){
	mystack.push(model_view_matrice);
	glBindVertexArray(vao_ID_[8]);
	mat4 instance = (
		Translate(10.0, 0.0, 38.0)
		*Scale(ARM_UPPER_wid, ARM_UPPER_H, ARM_UPPER_wid)
		);
	glUniformMatrix4fv(model_view_int, 1, GL_TRUE, model_view_matrice * instance);
	glDrawArrays(GL_TRIANGLES, 0, vertice_BOARD);
	model_view_matrice = mystack.pop();
}

void tiledropper(){
	mystack.push(model_view_matrice);
	glBindVertexArray(vao_ID_[2]);
	mat4 instance = (
		Scale(1.0, 1.0, 1.0) *
		Translate(-198.0, -660.0, 33.0)
		);
	glUniformMatrix4fv(model_view_int, 1, GL_TRUE, model_view_matrice * instance);
	glDrawArrays(GL_TRIANGLES, 0, 4*36);
	model_view_matrice = mystack.pop();
}

void boardGridTileDraw(){
	glBindVertexArray(vao_ID_[1]); 
	glDrawArrays(GL_TRIANGLES, 0, 1200); 

	if(isDropped){
		glBindVertexArray(vao_ID_[2]); 
		glDrawArrays(GL_TRIANGLES, 0, 4*36); 
	}

	glBindVertexArray(vao_ID_[3]); 
	glDrawArrays(GL_TRIANGLES, 0, 1200); 

	glBindVertexArray(vao_ID_[4]); 
	glDrawArrays(GL_TRIANGLES, 0, 1200); 

	glBindVertexArray(vao_ID_[5]); 
	glDrawArrays(GL_TRIANGLES, 0, 1200); 

	glBindVertexArray(vao_ID_[6]); 
	glDrawArrays(GL_TRIANGLES, 0, 1200); 

	glBindVertexArray(vao_ID_[7]); 
	glDrawArrays(GL_TRIANGLES, 0, 1200); 

	glBindVertexArray(vao_ID_[0]); 
	glDrawArrays(GL_LINES, 0, 128); 
}



void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	//SOMETHING WRONG WITH PROJECTION_VIEW_MATRICE
	projection_view_matrice = Perspective(45.0, 1.0*xsize/ysize, 5.0, 300.0); 

	mat4 M = mat4();
	M *= Translate(0.0, 10.0, 0);
	M *= Scale(1.0/33.0, 1.0/33.0, 1.0/33.0);
	M *= Translate(-200.0, -360.0, 0.0);

	model_view_matrice = projection_view_matrice * camera_view_matrice * M;

	glUniformMatrix4fv(model_view_int, 1, GL_TRUE, model_view_matrice);

	boardGridTileDraw();
	
	base();

	mystack.push(model_view_matrice);
	model_view_matrice *= ( 
		Translate(0.0, CUBE_base_h, 0.0) *
		RotateZ(ang_gamma[low_a])
		);

	lower();

	model_view_matrice *= (
		Translate(0.0, ARM_LOW_H, 0.0) *
		RotateZ(ang_gamma[up_a])
		);

	upper();
	
	model_view_matrice *= (
		Translate(0.0, ARM_UPPER_H, 0.0) * 
		RotateZ(-ang_gamma[up_a]) *
		RotateZ(-ang_gamma[low_a])
		);

	

	if(!isDropped) tiledropper();
	
	model_view_matrice = mystack.pop();

	glutSwapBuffers();

}


//reshape function
void reshape(GLsizei w, GLsizei h)
{
	xsize = w;
	ysize = h;
	zsize = w;	
	glViewport(0, 0, w, h);
}

void shuffleColors(){
	vec4 temp[6];
	memcpy(temp, other_col, 96);
	memmove(other_col, other_col+6, 288);
	memcpy(other_col+18, temp, 96);

	//find out what bind exactly does
	glBindBuffer(GL_ARRAY_BUFFER, vboID_[5]); 
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(other_col), other_col); 

}

//moving it down
void dropTile(){ 
	vec2 cur_tilepos = getTilePosition();
	int x_pos = cur_tilepos.x;
	int y_pos = cur_tilepos.y;

	CURR_TITLE_GLOB.x = x_pos-1;
	CURR_TITLE_GLOB.y = y_pos-1;

	update_the_board();
	isDropped = true;
	
}

bool is_allowed(vec2 cur_tilepos){
	bool result = true;
	int x_pos = cur_tilepos.x;
	int y_pos = cur_tilepos.y;

	vec2 all_cell_pos[4] = shapes[current_tile_type][current_tile_orientation]; 

	//loading all elements
	for(int j = 0; j<4; ++j){ 
		all_cell_pos[j].x += x_pos;
		all_cell_pos[j].y += y_pos;
	}



	for(int i = 0; i<4; i++){
		if( all_cell_pos[i].x < 1 
			|| all_cell_pos[i].x > 10 
			|| all_cell_pos[i].y < 1 
			|| all_cell_pos[i].y > 20) result = false;

		if(board[(int)all_cell_pos[i].x-1][(int)all_cell_pos[i].y-1] == true) result = false;
	}

	return result;	
}


void special_up(int key, int x, int y){
	switch(key){
		case GLUT_KEY_LEFT:
			rate_ = 0.0f;
			break;
		case GLUT_KEY_RIGHT:
			rate_ = 0.0f;
			break;
	}
}

void special(int key, int x, int y)
{
	switch(key)
	{
		case GLUT_KEY_UP:
			if(isDropped){
				rotate();
				update_the_board();
			}
			break;
		case GLUT_KEY_DOWN:
			if(isDropped){
				rotate();
				update_the_board();
			}
			break;
		case GLUT_KEY_RIGHT:
			if (glutGetModifiers() & GLUT_ACTIVE_CTRL) {
				ax_ = Yaxis;
				rate_ = -2.0f;
				camera_view_matrice *= RotateY(rate_);
				break;
			}
			else if(isDropped){
				move_dir(vec2(1,0));
				update_the_board();
			}
			break;
		case GLUT_KEY_LEFT:
			if(glutGetModifiers() & GLUT_ACTIVE_CTRL) {
				ax_ = Yaxis;
				rate_ = 2.0f;
				camera_view_matrice *= RotateY(rate_);
				break;
			}
			else if(isDropped){
				move_dir(vec2(-1,0));	
				update_the_board();
			}
			break;
		
			
	}
}


void key_inputs(unsigned char key, int x, int y)
{
	switch(key) 
	{
		case 'w':
			my_new_angle = up_a;
			if(ang_gamma[my_new_angle] < 0) ang_gamma[my_new_angle] += 1;			
			if(!isDropped) update_the_board();
			break;
		case 'q':
			exit(EXIT_SUCCESS);
			break;
		case 's':
			my_new_angle = up_a;
			if(ang_gamma[my_new_angle] > -180) ang_gamma[my_new_angle] -= 1;
			if(!isDropped) update_the_board();
			break;
		
		case 'd':
			my_new_angle = low_a;
			if(ang_gamma[my_new_angle] > -90) ang_gamma[my_new_angle] -= 1;
			if(!isDropped) update_the_board();
			break;
		case ' ':
			if(!isDropped && is_allowed(getTilePosition())){
				dropTile();
			} 
			break;
		case 'r': 
			here_we_go_again();
			break;
		case 'a':
			my_new_angle = low_a;
			if (ang_gamma[my_new_angle] < 90) ang_gamma[my_new_angle] += 1;
			if (!isDropped) update_the_board();
			break;
	}	
	glutPostRedisplay();
}


void idle(void)
{
	glutPostRedisplay();
}

void timedMove(int t){ 
	if(isDropped){
		move_dir(vec2(0, -1));
		update_the_board();
	}
	glutTimerFunc(1000, timedMove, 0);
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(xsize, ysize);
	glutInitWindowPosition(680, 178); 
	glutCreateWindow("Game");
	glewInit();
	init();
	// Callback functions
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutSpecialFunc(special);
	glutSpecialUpFunc(special_up);
	glutKeyboardFunc(key_inputs);
	glutIdleFunc(idle);
	glutTimerFunc(1000, timedMove, 0);

	glutMainLoop(); // Start main loop
	return 0;
}

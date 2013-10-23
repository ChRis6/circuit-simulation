#ifndef NODES_H
#define NODES_H

enum{
	NODE_RCL_TYPE = 0,
	NODE_SOURCE_V_TYPE,
	NODE_SOURCE_I_TYPE,
	NODE_MOSFET_TYPE,
	NODE_BJT_TYPE
};

/*
 * R: resistor
 * C: capacity
 * L: inductance
 */
typedef struct rcl{

	char name[10]; // these should be arrays
	char node1[10];
	char node2[10];

	double value; 
}RCL_T;

/* Voltage source.DC for now */
typedef struct source_v {
	char name[10];
	char node1[10];
	char node2[10];

	double value;
}SOURCE_V_T;

/* MOSFET transistor*/
typedef struct mos{
	char name[10];
	char drain[10];
	char gate[10];
	char source[10];
	char body[10];

	/* model name */

	double l;
	double w;

}MOSFET_T;

/* bipolar junction transistor*/
typedef struct bjt{

	char name[10];
	char collector[10];
	char base[10];
	char emitter[10];

	/* model name */

	/* area*/
}BJT_T;


/*
 * Higher level base node */
typedef union node{
	BJT_T bjt;
	MOSFET_T mosfet;
	SOURCE_V_T source_v;
	RCL_T rcl;
}NODE;
#endif
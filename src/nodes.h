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
 * L: phnio
 */
typedef struct rcl{

	char* name;
	char* node1;
	char* node2;

	double value; 
}RCL_T;

/* Voltage source.DC for now */
typedef struct source_v {
	char* name;
	char* node1;
	char* node2;

	double value;
}SOURCE_V_T;

/* MOSFET transistor*/
typedef struct mos{
	char* name;
	char* drain;
	char* gate;
	char* source;
	char* body;

	/* model name */

	double l;
	double w;

}MOSFET_T;

/* bipolar junction transistor*/
typedef struct bjt{

	char* name;
	char* collector;
	char* base;
	char* emitter;

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
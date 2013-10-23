#ifndef NODES_H
#define NODES_H

enum{
	//NODE_RCL_TYPE = 0,
	NODE_RESISTANCE_TYPE = 0 ,
	NODE_CAPACITY_TYPE,
	NODE_INDUCTANCE_TYPE,
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
typedef struct resistance{

	char name[10]; // these should be arrays
	int node1;
	int node2;

	double value; 
}RESISTANCE_T;


typedef struct capacity{

	char name[10]; // these should be arrays
	int node1;
	int node2;

	double value; 
}CAPACITY_T;

typedef struct inductance{

	char name[10]; // these should be arrays
	int node1;
	int node2;

	double value; 
}INDUCTANCE_T;

/* Voltage source.DC for now */
typedef struct source_v {
	char name[10];
	int node1;
	int node2;

	double value;
}SOURCE_V_T;

typedef struct source_i{
	char name[10];
	int node1;
	int node2;

	double value;
}SOURCE_I_T;
/* MOSFET transistor*/
typedef struct mos{
	char name[10];
	int drain;
	int gate;
	int source;
	int body;

	/* model name */

	double l;
	double w;

}MOSFET_T;

/* bipolar junction transistor*/
typedef struct bjt{

	char name[10];
	int collector;
	int base;
	int emitter;

	/* model name */

	/* area*/
}BJT_T;

/* diode */
typedef struct diode{
	char name[10];
	int node1;
	int node2;

	/* area */

	/* model name */

}DIODE_T;

/*
 * Higher level base node */
typedef union node{
	BJT_T bjt;
	MOSFET_T mosfet;
	SOURCE_V_T source_v;
	SOURCE_I_T source_i;
	RESISTANCE_T resistance;
	CAPACITY_T capacity;
	INDUCTANCE_T inductance;
	DIODE_T diode;
}NODE;
#endif
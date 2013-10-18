__kernel void rng(__global int* data) {

	int z2=0;

    	int z =0;
	for(z = 0; z<9; z++)
		data[z2] = data[z2] ^ 1;

}

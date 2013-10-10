__kernel void rng(__global int* data) {
        int z =0;
	for(z = 0; z<10091; z++)
		data[0] = data[0] ^ 1;
}

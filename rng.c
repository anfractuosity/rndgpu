#define PROGRAM_FILE "rng.cl"
#define KERNEL_FUNC "rng"
#define ARRAY_SIZE 1
#include <CL/cl.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>




/* Create program from a file and compile it */
cl_program build_program(cl_context ctx, cl_device_id dev, const char *filename) {
	cl_program program;
	FILE *program_handle;
	char *program_buffer, *program_log;
	size_t program_size, log_size;
	int err;

	/* Read program file and place content into buffer */
	program_handle = fopen(filename, "r");
	if (program_handle == NULL) {
		perror("Couldn't find the program file");
		exit(1);
	}
	fseek(program_handle, 0, SEEK_END);
	program_size = ftell(program_handle);
	rewind(program_handle);
	program_buffer = (char *)malloc(program_size + 1);
	program_buffer[program_size] = '\0';
	fread(program_buffer, sizeof(char), program_size, program_handle);
	fclose(program_handle);

	/* Create program from file */
	program = clCreateProgramWithSource(ctx, 1,
					    (const char **)&program_buffer,
					    &program_size, &err);
	if (err < 0) {
		perror("Couldn't create the program");
		exit(1);
	}
	free(program_buffer);

	/* Build program */
	err = clBuildProgram(program, 0, NULL, NULL, NULL, NULL);
	if (err < 0) {

		/* Find size of log and print to std output */
		clGetProgramBuildInfo(program, dev, CL_PROGRAM_BUILD_LOG,
				      0, NULL, &log_size);
		program_log = (char *)malloc(log_size + 1);
		program_log[log_size] = '\0';
		clGetProgramBuildInfo(program, dev, CL_PROGRAM_BUILD_LOG,
				      log_size + 1, program_log, NULL);
		printf("%s\n", program_log);
		free(program_log);
		exit(1);
	}
	return program;
}

void exitOnFail(cl_int status, const char* message)
{
    if (CL_SUCCESS != status)
    {
        printf("error: %s\n", message);
        exit(-1);
    }
}

int main(int argc, char *argv[])
{
    // return code used by OpenCL API
    cl_int status;

cl_program program;
cl_mem input_buffer;
    // wait event synchronization handle used by OpenCL API
    cl_event event;
 
    cl_int err;

    ////////////////////////////////////////
    // OpenCL platforms

    // determine number of platforms
    cl_uint numPlatforms;
    status = clGetPlatformIDs(0, NULL, &numPlatforms);
    exitOnFail(status, "number of platforms");

    // get platform IDs
    cl_platform_id platformIDs[numPlatforms];
    status = clGetPlatformIDs(numPlatforms, platformIDs, NULL);
    exitOnFail(status, "get platform IDs");

    ////////////////////////////////////////
    // OpenCL devices

    // look for a CPU and GPU compute device
    cl_platform_id cpuPlatformID, gpuPlatformID;
    cl_device_id cpuDeviceID, gpuDeviceID;
    int isCPU = 0, isGPU = 0;

    // iterate over platforms
    for (size_t i = 0; i < numPlatforms; i++)
    {
        // determine number of devices for a platform
        cl_uint numDevices;
        status = clGetDeviceIDs(platformIDs[i],
                                CL_DEVICE_TYPE_ALL,
                                0,
                                NULL,
                                &numDevices);
        if (CL_SUCCESS == status)
        {
            // get device IDs for a platform
            cl_device_id deviceIDs[numDevices];
            status = clGetDeviceIDs(platformIDs[i],
                                    CL_DEVICE_TYPE_ALL,
                                    numDevices,
                                    deviceIDs,
                                    NULL);
            if (CL_SUCCESS == status)
            {
                // iterate over devices
                for (size_t j = 0; j < numDevices; j++)
                {
                    cl_device_type deviceType;
                    status = clGetDeviceInfo(deviceIDs[j],
                                             CL_DEVICE_TYPE,
                                             sizeof(cl_device_type),
                                             &deviceType,
                                             NULL);
                    if (CL_SUCCESS == status)
                    {
                        // first CPU device
                        if (!isCPU && (CL_DEVICE_TYPE_CPU & deviceType))
                        {
                            isCPU = 1;
                            cpuPlatformID = platformIDs[i];
                            cpuDeviceID = deviceIDs[j];
                        }

                        // first GPU device
                        if (!isGPU && (CL_DEVICE_TYPE_GPU & deviceType))
                        {
                            isGPU = 1;
                            gpuPlatformID = platformIDs[i];
                            gpuDeviceID = deviceIDs[j];
                        }
                    }
                }
            }
        }
    }

    // pick GPU device if it exists, otherwise use CPU
    cl_platform_id platformID;
    cl_device_id deviceID;

    if (isGPU)
    {
        platformID = gpuPlatformID;
        deviceID = gpuDeviceID;
//	printf("Using GPU of platform %d\n",platformID);
    }
    else if (isCPU)
    {
        platformID = cpuPlatformID;
        deviceID = cpuDeviceID;
    }
    else
    {
        // no devices found
        exitOnFail(CL_DEVICE_NOT_FOUND, "no devices found");
    }







   int data[ARRAY_SIZE];
  int i =0;

/* Initialize data */
	for (i = 0; i < ARRAY_SIZE; i++) {
		data[i] = 0;
	}











    ////////////////////////////////////////
    // OpenCL context

    cl_context_properties props[] = { CL_CONTEXT_PLATFORM,
                                      (cl_context_properties) platformID,
                                      0 };
    cl_context context = clCreateContext(props,
                                         1,
                                         &deviceID,
                                         NULL,
                                         NULL,
                                         &status);
    exitOnFail(status, "create context");

    ////////////////////////////////////////
    // OpenCL command queue

input_buffer =
	    clCreateBuffer(context, CL_MEM_COPY_HOST_PTR,
			   ARRAY_SIZE * sizeof(int), data, &status);

   exitOnFail(status, "create input buffer");


    cl_command_queue queue = clCreateCommandQueue(context,
                                                  deviceID,
                                                  0,
                                                  &status);
    exitOnFail(status, "create command queue");







    program = build_program(context, deviceID, PROGRAM_FILE);



    cl_kernel kernel;


kernel = clCreateKernel(program, KERNEL_FUNC, &err);
	if (err < 0) {
		perror("Couldn't create a kernel");
		exit(1);
	};

	status = clSetKernelArg(kernel, 0, sizeof(cl_mem), &input_buffer);
    exitOnFail(status, "set arg");


    ////////////////////////////////////////
    // OpenCL enqueue kernel and wait

    // N work-items in groups of 4
    const size_t N = 1024*20; 
    const size_t groupsize = 4;
    const size_t global[] = { N }, local[] = { groupsize };


	while (1) {
    // enqueue kernel
    status = clEnqueueNDRangeKernel(queue,
                                    kernel,
                                    sizeof(global)/sizeof(size_t),
                                    NULL,
                                    global,
                                    local,
                                    0,
                                    NULL,
                                    &event);
    exitOnFail(status, "enqueue kernel");

    // wait for kernel, this forces execution
    status = clWaitForEvents(1, &event);
    exitOnFail(status, "wait for enqueue kernel");
   // clReleaseEvent(event);

    ////////////////////////////////////////
    // OpenCL read back buffer from device

    // data transfer for array Y
   /* status = clEnqueueReadBuffer(queue,
                                 memY,
                                 CL_FALSE,
                                 0,
                                 N * sizeof(float),
                                 cpuY,
                                 0,
                                 NULL,
                                 &event);*/


	status =
		    clEnqueueReadBuffer(queue, input_buffer, CL_TRUE, 0,
					sizeof(data), data, 0, NULL, NULL);
		if (err < 0) {
			perror("Couldn't read the buffer");
			exit(1);
		}

for (size_t i = 0; i < ARRAY_SIZE; i++)
    {
        printf("%d\n", (int)data[i]);
	fflush(stdout);
    }
   }

   exitOnFail(status, "read Y from device");
    status = clWaitForEvents(1, &event);
    exitOnFail(status, "wait for read Y from device");
    clReleaseEvent(event);

    ////////////////////////////////////////
    // OpenCL cleanup

    clReleaseKernel(kernel);
    clReleaseProgram(program);
    //clReleaseMemObject(memX);
    //clReleaseMemObject(memY);
    clReleaseCommandQueue(queue);
    clReleaseContext(context);

    ////////////////////////////////////////
    // print computed result

    

    exit(0);
}

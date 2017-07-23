#include <stdio.h>
#include <fcntl.h>

#include <gphoto2/gphoto2-camera.h>

Camera *camera;
GPContext *context;

void error_func (GPContext *context, const char *format, void *data) {
 fprintf(stderr, "*** Contexterror ***\n");
 // vfprintf(stderr, format, args);
 fprintf(stderr, "\n");
}

void message_func (GPContext *context, const char *format, void *data) {
 // vprintf(format, args);
 printf("\n");
}

int capture (const char *filename) {
 int fd, retval;
 CameraFile *file;
 CameraFilePath camera_file_path;

 // this was done in the libphoto2 example code, but doesn't seem to be necessary
 // NOP: This gets overridden in the library to /capt0000.jpg
 //snprintf(camera_file_path.folder, 1024, "/");
 //snprintf(camera_file_path.name, 128, "foo.jpg");

 // take a shot
 retval = gp_camera_capture(camera, GP_CAPTURE_IMAGE, &camera_file_path, context);
 
 if (retval) {
  // do some error handling, probably return from this function
 }

 printf("Pathname on the camera: %s/%s\n", camera_file_path.folder, camera_file_path.name);

 fd = open(filename, O_CREAT | O_WRONLY, 0644);
 // create new CameraFile object from a file descriptor
 retval = gp_file_new_from_fd(&file, fd);
 
 if (retval) {
  // error handling
 }

 // copy picture from camera
 retval = gp_camera_file_get(camera, camera_file_path.folder, camera_file_path.name,
   GP_FILE_TYPE_NORMAL, file, context);
 
 if (retval) {
  // error handling
 }

 printf("Deleting\n");
 // remove picture from camera memory
 retval = gp_camera_file_delete(camera, camera_file_path.folder, camera_file_path.name,
   context);
 
 if (retval) {
  // error handling
 }

 // free CameraFile object
 gp_file_free(file);

 // Code from here waits for camera to complete everything.
 // Trying to take two successive captures without waiting
 // will result in the camera getting randomly stuck.
 int waittime = 2000;
 CameraEventType type;
 void *data;

 printf("Wait for events from camera\n");
 while(1) {
  retval = gp_camera_wait_for_event(camera, waittime, &type, &data, context);
  if(type == GP_EVENT_TIMEOUT) {
   break;
  }
  else if (type == GP_EVENT_CAPTURE_COMPLETE) {
   printf("Capture completed\n");
   waittime = 100;
  }
  else if (type != GP_EVENT_UNKNOWN) {
   printf("Unexpected event received from camera: %d\n", (int)type);
  }
 }
 return 0;
}

int main (int argc, char *argv[]) {
 gp_camera_new (&camera);
 context = gp_context_new();

 // set callbacks for camera messages
 gp_context_set_error_func(context, error_func, NULL);
 gp_context_set_message_func(context, message_func, NULL);

 //This call will autodetect cameras, take the first one from the list and use it
 printf("Camera init. Can take more than 10 seconds depending on the "
  "memory card's contents (remove card from camera to speed up).\n");
 int ret = gp_camera_init(camera, context);
 if (ret < GP_OK) {
  printf("No camera auto detected.\n");
  gp_camera_free(camera);
  return 1;
 }

 // take 10 shots
 char filename[256];
 int const nShots = 10;
 int i;

 // do some capturing
 for (i = 0; i < nShots; i++) {
  snprintf(filename, 256, "shot-%04d.nef", i);
  printf("Capturing to file %s\n", filename);
  capture(filename);
 }

 // close camera
 gp_camera_unref(camera);
 gp_context_unref(context);

 return 0;
}
#include <linux/cdev.h>
#include <linux/delay.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/gpio.h> //GPIO
#include <linux/init.h>
#include <linux/kdev_t.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/uaccess.h> //copy_to/from_user()
#include <linux/unistd.h>


// LED is connected to this GPIO


#define GPIO_1_3V3 (1) //test
//7-seg
#define GPIO_2_A (2)
#define GPIO_3_B (3)
#define GPIO_4_C (4)
#define GPIO_17_D (17)
#define GPIO_27_E (27)
#define GPIO_22_F (22)
#define GPIO_10_G (10)


#define GPIO_16 (16)
#define GPIO_20 (20)
#define GPIO_21 (21)

#define GPIO_13 (13)
#define GPIO_19 (19)
#define GPIO_26 (26)

#define GPIO_12 (12)

#define GPIO_11 (11)
#define GPIO_5 (5)
#define GPIO_6 (6)

#define GPIO_14_A (14)
#define GPIO_15_B (15)
#define GPIO_18_C (18)
#define GPIO_23_D (23)
#define GPIO_24_E (24)
#define GPIO_25_F (25)
#define GPIO_8_G (8)



dev_t dev = 0;
static struct class *dev_class;
static struct cdev etx_cdev;
static int __init etx_driver_init(void);
static void __exit etx_driver_exit(void);
/*************** Driver functions **********************/
static int etx_open(struct inode *inode, struct file *file);
static int etx_release(struct inode *inode, struct file *file);
static ssize_t etx_read(struct file *filp, char __user *buf, size_t len,loff_t *off);
static ssize_t etx_write(struct file *filp, const char *buf, size_t len,loff_t *off);
/******************************************************/


/** 7 Segment array **/
int segment_catagory[10][7] ={
  //{a,b,c,d,e,f,g}, pin level for each number.
  {1,1,1,1,1,1,0}, // 0 segment_catagory[0]
  {0,1,1,0,0,0,0}, // 1 segment_catagory[1]
  {1,1,0,1,1,0,1}, // 2         .
  {1,1,1,1,0,0,1}, // 3         .
  {0,1,1,0,0,1,1}, // 4
  {1,0,1,1,0,1,1}, // 5 
  {1,0,1,1,1,1,1}, // 6 
  {1,1,1,0,0,0,0}, // 7 
  {1,1,1,1,1,1,1}, // 8
  {1,1,1,1,0,1,1} // 9
};

void set_7segment_bedroom(int *pinLevel){

  gpio_set_value(GPIO_2_A,pinLevel[0]);
  gpio_set_value(GPIO_3_B,pinLevel[1]);
  gpio_set_value(GPIO_4_C,pinLevel[2]);
  gpio_set_value(GPIO_17_D,pinLevel[3]);
  gpio_set_value(GPIO_27_E,pinLevel[4]);
  gpio_set_value(GPIO_22_F,pinLevel[5]);
  gpio_set_value(GPIO_10_G,pinLevel[6]);

}

void set_7segment_livingroom(int *pinLevel){

  gpio_set_value(GPIO_14_A,pinLevel[0]);
  gpio_set_value(GPIO_15_B,pinLevel[1]);
  gpio_set_value(GPIO_18_C,pinLevel[2]);
  gpio_set_value(GPIO_23_D,pinLevel[3]);
  gpio_set_value(GPIO_24_E,pinLevel[4]);
  gpio_set_value(GPIO_25_F,pinLevel[5]);
  gpio_set_value(GPIO_8_G,pinLevel[6]);

}


// File operation structure
static struct file_operations fops = {
    .owner = THIS_MODULE,
    .read = etx_read,
    .write = etx_write,
    .open = etx_open,
    .release = etx_release,
};

static int etx_open(struct inode *inode, struct file *file) {
  pr_info("Device File Opened...!!!\n");
  return 0;
}

static int etx_release(struct inode *inode, struct file *file) {
  pr_info("Device File Closed...!!!\n");
  return 0;
}

static ssize_t etx_read(struct file *filp, char __user *buf, size_t len,loff_t *off) {
  uint8_t gpio_state = 0;

  // reading GPIO value
  gpio_state = gpio_get_value(GPIO_17_D);

  // write to user
  len = 1;
  if (copy_to_user(buf, &gpio_state, len) > 0) {
    pr_err("ERROR: Not all the bytes have been copied to user\n");
  }

  pr_info("Read function : GPIO_21 = %d \n", gpio_state);

  return 0;
}

static ssize_t etx_write(struct file *filp, const char __user *buf, size_t len,loff_t *off) {

  uint8_t rec_buf[50] = {0}; // uint_8 is a usinged char
  
  // rcv_Seq is a counter for rec_data from user space. (student_ID)
  int rcv_Seq = 0;

  if (copy_from_user(rec_buf, buf, len) > 0) {
    pr_err("ERROR: Not all the bytes have been copied from user\n");
  }

  // studentID have 9 numbers (0~8)
  while(rec_buf[rcv_Seq] != '\0'){
    // printf ascii code of student_ID
    pr_info("Write Function : received = %d\n", rec_buf[rcv_Seq]);
    pr_info("value = %d\n", rec_buf[rcv_Seq]-'0');
    rcv_Seq++;
  }

  /*            bedroom            */ 

  // light
  if((rec_buf[3]-'0') > 0){
    gpio_set_value(GPIO_13,1);
  }
  else{
    gpio_set_value(GPIO_13,0);
  }
  // fan
  if((rec_buf[5]-'0') > 0){
    gpio_set_value(GPIO_19,1);
  }
  else{
    gpio_set_value(GPIO_19,0);
  }
  // curtain
  if((rec_buf[7]-'0') > 0){
    gpio_set_value(GPIO_26,1);
  }
  else{
    gpio_set_value(GPIO_26,0);
  }
  /*            living room            */
  
  // light
  if((rec_buf[12]-'0') > 0){
    gpio_set_value(GPIO_21,1);
  }
  else{
    gpio_set_value(GPIO_21,0);
  }
  // fan
  if((rec_buf[14]-'0') > 0){
    gpio_set_value(GPIO_20,1);
  }
  else{
    gpio_set_value(GPIO_20,0);
  }
  // curtain
  if((rec_buf[16]-'0') > 0){
    gpio_set_value(GPIO_16,1);
  }
  else{
    gpio_set_value(GPIO_16,0);
  }


  /*            kitchen             */

  // light
  if((rec_buf[18]-'0') > 0){
    gpio_set_value(GPIO_11,1);
  }
  else{
    gpio_set_value(GPIO_11,0);
  }

  /*            bathroom  &  door           */


  //aircon
  if((rec_buf[20]-'0') > 0){    
    gpio_set_value(GPIO_5,1); 
  }
  else{
    gpio_set_value(GPIO_5,0);    
  }

    // light
  if((rec_buf[23]-'0') > 0){
    gpio_set_value(GPIO_6,1);
  }
  else{
    gpio_set_value(GPIO_6,0);
  }
      // door
  if((rec_buf[25]-'0') > 0){
    gpio_set_value(GPIO_12,1);
  }
  else{
    gpio_set_value(GPIO_12,0);
  }

  // airconditioner Value
  // first airconditioner
  int segFirstNum = rec_buf[0]-'0';
  int segSecondNum = rec_buf[1]-'0';
  if(segFirstNum != 0){
    int count = 0;
    while(count < 1){
        set_7segment_bedroom(segment_catagory[segFirstNum]);
        fsleep(1000000);
        set_7segment_bedroom(segment_catagory[segSecondNum]); 
        fsleep(1000000);
        count++;
    }
  }
  else{
    set_7segment_bedroom(segment_catagory[0]);
  }

  // second airconditioner

  segFirstNum = rec_buf[9]-'0';
  segSecondNum = rec_buf[10]-'0';;

  if(segFirstNum != 0){
    int count = 0;
    while(count < 1){
        set_7segment_livingroom(segment_catagory[segFirstNum]);
        fsleep(1000000);
        set_7segment_livingroom(segment_catagory[segSecondNum]); 
        fsleep(1000000);
        count++;
    }
  }
  else{
    set_7segment_livingroom(segment_catagory[0]);
  }


  return len;
}
/*
** Module Init function
*/
static int __init etx_driver_init(void) {
  /*Allocating Major number*/
  if ((alloc_chrdev_region(&dev, 0, 1, "etx_Dev")) < 0) {
    pr_err("Cannot allocate major number\n");
    goto r_unreg;
  }
  pr_info("start init ...\n");
  pr_info("Major = %d Minor = %d \n", MAJOR(dev), MINOR(dev));
  /*Creating cdev structure*/
  cdev_init(&etx_cdev, &fops);
  /*Adding character device to the system*/
  if ((cdev_add(&etx_cdev, dev, 1)) < 0) {
    pr_err("Cannot add the device to the system\n");
    goto r_del;
  }
  /*Creating struct class*/
  if ((dev_class = class_create(THIS_MODULE, "etx_class")) == NULL) {
    pr_err("Cannot create the struct class\n");
    goto r_class;
  }
  /*Creating device*/
  if ((device_create(dev_class, NULL, dev, NULL, "etx_device")) == NULL) {
    pr_err("Cannot create the Device \n");
    goto r_device;
  }

  // Checking the GPIO is valid or not
  if (gpio_is_valid(GPIO_10_G) == false) {
    pr_err("GPIO %d is not valid\n", GPIO_10_G);
    goto r_device;
  }
  if (gpio_is_valid(GPIO_22_F) == false) {
    pr_err("GPIO %d is not valid\n", GPIO_22_F);
    goto r_device;
  }
  if (gpio_is_valid(GPIO_27_E) == false) {
    pr_err("GPIO %d is not valid\n", GPIO_27_E);
    goto r_device;
  }
  if (gpio_is_valid(GPIO_17_D) == false) {
    pr_err("GPIO %d is not valid\n", GPIO_17_D);
    goto r_device;
  }
  if (gpio_is_valid(GPIO_4_C) == false) {
    pr_err("GPIO %d is not valid\n", GPIO_4_C);
    goto r_device;
  }
  if (gpio_is_valid(GPIO_3_B) == false) {
    pr_err("GPIO %d is not valid\n", GPIO_3_B);
    goto r_device;
  }
  if (gpio_is_valid(GPIO_2_A) == false) {
    pr_err("GPIO %d is not valid\n", GPIO_2_A);
    goto r_device;
  }
  if (gpio_is_valid(GPIO_1_3V3) == false) {
    pr_err("GPIO %d is not valid\n", GPIO_1_3V3);
    goto r_device;
  }

    if (gpio_is_valid(GPIO_21) == false) {
    pr_err("GPIO %d is not valid\n", GPIO_21);
    goto r_device;
  }
    if (gpio_is_valid(GPIO_20) == false) {
    pr_err("GPIO %d is not valid\n", GPIO_20);
    goto r_device;
  }
    if (gpio_is_valid(GPIO_16) == false) {
    pr_err("GPIO %d is not valid\n", GPIO_16);
    goto r_device;
  }
    if (gpio_is_valid(GPIO_26) == false) {
    pr_err("GPIO %d is not valid\n", GPIO_26);
    goto r_device;
  }
    if (gpio_is_valid(GPIO_19) == false) {
    pr_err("GPIO %d is not valid\n", GPIO_19);
    goto r_device;
  }
    if (gpio_is_valid(GPIO_13) == false) {
    pr_err("GPIO %d is not valid\n", GPIO_13);
    goto r_device;
  }

  if (gpio_is_valid(GPIO_13) == false) {
    pr_err("GPIO %d is not valid\n", GPIO_13);
    goto r_device;
}

if (gpio_is_valid(GPIO_12) == false) {
    pr_err("GPIO %d is not valid\n", GPIO_12);
    goto r_device;
}

if (gpio_is_valid(GPIO_6) == false) {
    pr_err("GPIO %d is not valid\n", GPIO_6);
    goto r_device;
}

if (gpio_is_valid(GPIO_5) == false) {
    pr_err("GPIO %d is not valid\n", GPIO_5);
    goto r_device;
}

if (gpio_is_valid(GPIO_11) == false) {
    pr_err("GPIO %d is not valid\n", GPIO_11);
    goto r_device;
}

if (gpio_is_valid(GPIO_14_A) == false) {
    pr_err("GPIO %d is not valid\n", GPIO_14_A);
    goto r_device;
}

if (gpio_is_valid(GPIO_15_B) == false) {
    pr_err("GPIO %d is not valid\n", GPIO_15_B);
    goto r_device;
}

if (gpio_is_valid(GPIO_18_C) == false) {
    pr_err("GPIO %d is not valid\n", GPIO_18_C);
    goto r_device;
}

if (gpio_is_valid(GPIO_23_D) == false) {
    pr_err("GPIO %d is not valid\n", GPIO_23_D);
    goto r_device;
}

if (gpio_is_valid(GPIO_24_E) == false) {
    pr_err("GPIO %d is not valid\n", GPIO_24_E);
    goto r_device;
}

if (gpio_is_valid(GPIO_25_F) == false) {
    pr_err("GPIO %d is not valid\n", GPIO_25_F);
    goto r_device;
}

if (gpio_is_valid(GPIO_8_G) == false) {
    pr_err("GPIO %d is not valid\n", GPIO_8_G);
    goto r_device;
}




  // Requesting the GPIO
  if (gpio_request(GPIO_1_3V3, "GPIO_1") < 0) {
    pr_err("ERROR: GPIO %d request\n", GPIO_1_3V3);
    goto r_gpio;
  }
  if (gpio_request(GPIO_2_A, "GPIO_2") < 0) {
    pr_err("ERROR: GPIO %d request\n", GPIO_2_A);
    goto r_gpio;
  }
  if (gpio_request(GPIO_3_B, "GPIO_3") < 0) {
    pr_err("ERROR: GPIO %d request\n", GPIO_3_B);
    goto r_gpio;
  }
  if (gpio_request(GPIO_4_C, "GPIO_4") < 0) {
    pr_err("ERROR: GPIO %d request\n", GPIO_4_C);
    goto r_gpio;
  }
  if (gpio_request(GPIO_17_D, "GPIO_17") < 0) {
    pr_err("ERROR: GPIO %d request\n", GPIO_17_D);
    goto r_gpio;
  }
  if (gpio_request(GPIO_27_E, "GPIO_27") < 0) {
    pr_err("ERROR: GPIO %d request\n", GPIO_27_E);
    goto r_gpio;
  }
  if (gpio_request(GPIO_22_F, "GPIO_22") < 0) {
    pr_err("ERROR: GPIO %d request\n", GPIO_22_F);
    goto r_gpio;
  }
  if (gpio_request(GPIO_10_G, "GPIO_10") < 0) {
    pr_err("ERROR: GPIO %d request\n", GPIO_10_G);
    goto r_gpio;
  }

    if (gpio_request(GPIO_21, "GPIO_21") < 0) {
    pr_err("ERROR: GPIO %d request\n", GPIO_21);
    goto r_gpio;
  }
    if (gpio_request(GPIO_20, "GPIO_20") < 0) {
    pr_err("ERROR: GPIO %d request\n", GPIO_20);
    goto r_gpio;
  }
    if (gpio_request(GPIO_16, "GPIO_16") < 0) {
    pr_err("ERROR: GPIO %d request\n", GPIO_16);
    goto r_gpio;
  }
    if (gpio_request(GPIO_26, "GPIO_26") < 0) {
    pr_err("ERROR: GPIO %d request\n", GPIO_26);
    goto r_gpio;
  }
    if (gpio_request(GPIO_19, "GPIO_19") < 0) {
    pr_err("ERROR: GPIO %d request\n", GPIO_19);
    goto r_gpio;
  }
    if (gpio_request(GPIO_13, "GPIO_13") < 0) {
    pr_err("ERROR: GPIO %d request\n", GPIO_13);
    goto r_gpio;
  }

  
if (gpio_request(GPIO_12, "GPIO_12") < 0) {
    pr_err("ERROR: GPIO %d request\n", GPIO_12);
    goto r_gpio;
}

if (gpio_request(GPIO_11, "GPIO_11") < 0) {
    pr_err("ERROR: GPIO %d request\n", GPIO_11);
    goto r_gpio;
}

if (gpio_request(GPIO_5, "GPIO_5") < 0) {
    pr_err("ERROR: GPIO %d request\n", GPIO_5);
    goto r_gpio;
}

if (gpio_request(GPIO_6, "GPIO_6") < 0) {
    pr_err("ERROR: GPIO %d request\n", GPIO_6);
    goto r_gpio;
}

if (gpio_request(GPIO_14_A, "GPIO_14_A") < 0) {
    pr_err("ERROR: GPIO %d request\n", GPIO_14_A);
    goto r_gpio;
}

if (gpio_request(GPIO_15_B, "GPIO_15_B") < 0) {
    pr_err("ERROR: GPIO %d request\n", GPIO_15_B);
    goto r_gpio;
}

if (gpio_request(GPIO_18_C, "GPIO_18_C") < 0) {
    pr_err("ERROR: GPIO %d request\n", GPIO_18_C);
    goto r_gpio;
}

if (gpio_request(GPIO_23_D, "GPIO_23_D") < 0) {
    pr_err("ERROR: GPIO %d request\n", GPIO_23_D);
    goto r_gpio;
}

if (gpio_request(GPIO_24_E, "GPIO_24_E") < 0) {
    pr_err("ERROR: GPIO %d request\n", GPIO_24_E);
    goto r_gpio;
}

if (gpio_request(GPIO_25_F, "GPIO_25_F") < 0) {
    pr_err("ERROR: GPIO %d request\n", GPIO_25_F);
    goto r_gpio;
}

if (gpio_request(GPIO_8_G, "GPIO_8_G") < 0) {
    pr_err("ERROR: GPIO %d request\n", GPIO_8_G);
    goto r_gpio;
}

  pr_info("request GPIO complete.\n");

  // configure the GPIO as output
  gpio_direction_output(GPIO_10_G, 0);
  gpio_direction_output(GPIO_22_F, 0);
  gpio_direction_output(GPIO_27_E, 0);
  gpio_direction_output(GPIO_17_D, 0);
  gpio_direction_output(GPIO_4_C, 0);
  gpio_direction_output(GPIO_3_B, 0);
  gpio_direction_output(GPIO_2_A, 0);
  gpio_direction_output(GPIO_1_3V3, 0);

  gpio_direction_output(GPIO_21, 0);
  gpio_direction_output(GPIO_20, 0);
  gpio_direction_output(GPIO_16, 0);
  gpio_direction_output(GPIO_26, 0);
  gpio_direction_output(GPIO_19, 0);

  gpio_direction_output(GPIO_13, 0);
  gpio_direction_output(GPIO_12, 0);
  gpio_direction_output(GPIO_11, 0);
  gpio_direction_output(GPIO_5, 0);
  gpio_direction_output(GPIO_6, 0);
  gpio_direction_output(GPIO_14_A, 0);
  gpio_direction_output(GPIO_15_B, 0);
  gpio_direction_output(GPIO_18_C, 0);
  gpio_direction_output(GPIO_23_D, 0);
  gpio_direction_output(GPIO_24_E, 0);
  gpio_direction_output(GPIO_25_F, 0);
  gpio_direction_output(GPIO_8_G, 0);


  pr_info("direction set complete.\n");
  /* Using this call the GPIO 21 will be visible in /sys/class/gpio/
  ** Now you can change the gpio values by using below commands also.
  ** echo 1 > /sys/class/gpio/gpio21/value (turn ON the LED)
  ** echo 0 > /sys/class/gpio/gpio21/value (turn OFF the LED)
  ** cat /sys/class/gpio/gpio21/value (read the value LED)
  **
  ** the second argument prevents the direction from being changed.
  */
  gpio_export(GPIO_10_G, false);
  gpio_export(GPIO_22_F, false);
  gpio_export(GPIO_27_E, false);
  gpio_export(GPIO_17_D, false);
  gpio_export(GPIO_4_C, false);
  gpio_export(GPIO_3_B, false);
  gpio_export(GPIO_2_A, false);
  gpio_export(GPIO_1_3V3, false);
  gpio_export(GPIO_21, false);
  gpio_export(GPIO_20, false);
  gpio_export(GPIO_16, false);
  gpio_export(GPIO_26, false);
  gpio_export(GPIO_19, false);
  gpio_export(GPIO_13, false);
  gpio_export(GPIO_12, false);
  gpio_export(GPIO_11, false);
  gpio_export(GPIO_5, false);
  gpio_export(GPIO_6, false);
  gpio_export(GPIO_14_A, false);
  gpio_export(GPIO_15_B, false);
  gpio_export(GPIO_18_C, false);
  gpio_export(GPIO_23_D, false);
  gpio_export(GPIO_24_E, false);
  gpio_export(GPIO_25_F, false);
  gpio_export(GPIO_8_G, false);

  pr_info("Device Driver Insert...Done!!!\n");
  return 0;
r_gpio:
  gpio_free(GPIO_10_G);
  gpio_free(GPIO_22_F);
  gpio_free(GPIO_27_E);
  gpio_free(GPIO_17_D);
  gpio_free(GPIO_4_C);
  gpio_free(GPIO_3_B);
  gpio_free(GPIO_2_A);
  gpio_free(GPIO_1_3V3);

  gpio_free(GPIO_21);
  gpio_free(GPIO_20);
  gpio_free(GPIO_16);
  gpio_free(GPIO_26);
  gpio_free(GPIO_19);
  gpio_free(GPIO_13);
  gpio_free(GPIO_12);
  gpio_free(GPIO_11);
  gpio_free(GPIO_5);
  gpio_free(GPIO_6);
  gpio_free(GPIO_14_A);
  gpio_free(GPIO_15_B);
  gpio_free(GPIO_18_C);
  gpio_free(GPIO_23_D);
  gpio_free(GPIO_24_E);
  gpio_free(GPIO_25_F);
  gpio_free(GPIO_8_G);

r_device:
  device_destroy(dev_class, dev);
r_class:
  class_destroy(dev_class);
r_del:
  cdev_del(&etx_cdev);
r_unreg:
  unregister_chrdev_region(dev, 1);

  return -1;
}
/*
** Module exit function
*/
static void __exit etx_driver_exit(void) {

  gpio_unexport(GPIO_10_G);
  gpio_free(GPIO_10_G);

  gpio_unexport(GPIO_22_F);
  gpio_free(GPIO_22_F);
  
  gpio_unexport(GPIO_27_E);
  gpio_free(GPIO_27_E);
  
  gpio_unexport(GPIO_17_D);
  gpio_free(GPIO_17_D);

  gpio_unexport(GPIO_4_C);
  gpio_free(GPIO_4_C);

  gpio_unexport(GPIO_3_B);
  gpio_free(GPIO_3_B);

  gpio_unexport(GPIO_2_A);
  gpio_free(GPIO_2_A);

  gpio_unexport(GPIO_1_3V3);
  gpio_free(GPIO_1_3V3);

  gpio_unexport(GPIO_21);
  gpio_free(GPIO_21);

  gpio_unexport(GPIO_20);
  gpio_free(GPIO_20);

  gpio_unexport(GPIO_16);
  gpio_free(GPIO_16);

  gpio_unexport(GPIO_26);
  gpio_free(GPIO_26);

  gpio_unexport(GPIO_19);
  gpio_free(GPIO_19);

  gpio_unexport(GPIO_13);
  gpio_free(GPIO_13);

  gpio_unexport(GPIO_12);
  gpio_free(GPIO_12);

  gpio_unexport(GPIO_11);
  gpio_free(GPIO_11);

  gpio_unexport(GPIO_5);
  gpio_free(GPIO_5);

  gpio_unexport(GPIO_6);
  gpio_free(GPIO_6);

  gpio_unexport(GPIO_14_A);
  gpio_free(GPIO_14_A);

  gpio_unexport(GPIO_15_B);
  gpio_free(GPIO_15_B);

  gpio_unexport(GPIO_18_C);
  gpio_free(GPIO_18_C);

  gpio_unexport(GPIO_23_D);
  gpio_free(GPIO_23_D);

  gpio_unexport(GPIO_24_E);
  gpio_free(GPIO_24_E);

  gpio_unexport(GPIO_25_F);
  gpio_free(GPIO_25_F);

  gpio_unexport(GPIO_8_G);
  gpio_free(GPIO_8_G);

  device_destroy(dev_class, dev);
  class_destroy(dev_class);
  cdev_del(&etx_cdev);
  unregister_chrdev_region(dev, 1);
  pr_info("Device Driver Remove...Done!!\n");
}
module_init(etx_driver_init);
module_exit(etx_driver_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("EmbeTronicX <embetronicx@gmail.com>");
MODULE_DESCRIPTION("A simple device driver - GPIO Driver");
MODULE_VERSION("1.32");

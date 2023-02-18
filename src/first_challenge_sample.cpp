#include "first_challenge_sample/first_challenge_sample.h"

FirstChallenge::FirstChallenge():private_nh_("~")
{
    private_nh_.param("hz", hz_, {10});
    odom_sub_ = nh_.subscribe("/roomba/odometry", 1, &FirstChallenge::odometry_callback, this);
    laser_sub_ = nh_.subscribe("/scan", 1, &FirstChallenge::laser_callback, this);
    cmd_vel_pub_ = nh_.advertise<roomba_500driver_meiji::RoombaCtrl>("/roomba/control", 1);
}

void FirstChallenge::odometry_callback(const nav_msgs::Odometry::ConstPtr& msg)
{
    odometry_ = *msg;
}

void FirstChallenge::laser_callback(const sensor_msgs::LaserScan::ConstPtr& msg)
{
    laser_ = *msg;
}

void FirstChallenge::run()
{
    cmd_vel_.mode = 11;
    cmd_vel_.cntl.linear.x = 0.2;
    cmd_vel_.cntl.angular.z = M_PI/6;

    cmd_vel_pub_.publish(cmd_vel_);
}

void FirstChallenge::process()
{
    ros::Rate loop_rate(hz_);
    while(ros::ok())
    {
        run();
        ros::spinOnce();
        loop_rate.sleep();
    }
}

int main(int argc, char** argv)
{
    ros::init(argc, argv, "first_challenge_sample");
    FirstChallenge first_challenge;
    first_challenge.process();
    return 0;
}

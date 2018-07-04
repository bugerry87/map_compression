#include "ros/ros.h"
#include "ros/console.h"

#include "pointmatcher/PointMatcher.h"
#include "pointmatcher/Timer.h"
#include "pointmatcher_ros/point_cloud.h"
#include "pointmatcher_ros/transform.h"
#include "nabo/nabo.h"
#include "eigen_conversions/eigen_msg.h"
#include "pointmatcher_ros/get_params_from_server.h"

#include <tf/transform_broadcaster.h>

#include <fstream>
#include <vector>
#include <algorithm>

#include <visualization_msgs/Marker.h>
#include <geometry_msgs/PoseStamped.h>
#include <nav_msgs/Path.h>

#include <numeric>

using namespace std;
using namespace PointMatcherSupport;

class loadProResult
{
    typedef PointMatcher<float> PM;
    typedef PM::DataPoints DP;

public:
    loadProResult(ros::NodeHandle &n);
    ~loadProResult();
    ros::NodeHandle& n;

    string loadResultDir;
    string loadMapName;
    string saveCloudName;

    DP mapCloud;

    int sectionNum;

};

loadProResult::~loadProResult()
{}

loadProResult::loadProResult(ros::NodeHandle& n):
    n(n),
    loadResultDir(getParam<string>("loadResultDir", ".")),
    loadMapName(getParam<string>("loadMapName", ".")),
    saveCloudName(getParam<string>("saveCloudName", ".")),
    sectionNum(getParam<int>("sectionNum", 0))
{
    //map
    mapCloud = DP::load(loadMapName);
    mapCloud.addDescriptor("salient_results", PM::Matrix::Zero(1, mapCloud.features.cols()));
    int rowLineResults = mapCloud.getDescriptorStartingRow("salient_results");

    //results
    // read all result files
    int temp;
    for (int y = 0; y < sectionNum; y++)
    {
        cout<<"File:  "<<y<<endl;
        string loadResultName = loadResultDir + std::to_string(y) + ".txt";
        ifstream in(loadResultName);
        while(!in.eof())
        {
            in>>temp;
            mapCloud.descriptors(rowLineResults, temp) = 1;
        }
        in.close();
    }

    mapCloud.save(saveCloudName);
    cout<<"Saved"<<endl;
}


int main(int argc, char **argv)
{

    ros::init(argc, argv, "loadProResult");
    ros::NodeHandle n;

    loadProResult loadproresult(n);

    // ugly code

    return 0;
}

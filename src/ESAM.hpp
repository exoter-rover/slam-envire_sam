/**\file ESAM.hpp
 *
 * Wrapper class to deal with iSAM2, Factor Graphs and envire graph
 *
 * @author Javier Hidalgo Carrio et. al
 * See LICENSE for the license information
 *
 */


#ifndef __ENVIRE_SAM_ESAM__
#define __ENVIRE_SAM_ESAM__

#include <fstream>

/** Rock Base Types **/
#include <base/Eigen.hpp>
#include <base/Pose.hpp>
#include <base/samples/Pointcloud.hpp>

/** Envire **/
#include <envire_core/all>

/** GTSAM TYPES **/
#include <gtsam/geometry/Pose3.h>
#include <gtsam/geometry/Point3.h>
#include <gtsam/geometry/Point2.h>
#include <gtsam/geometry/SimpleCamera.h>
#include <gtsam/nonlinear/Symbol.h>

/** GTSAM Factors **/
#include <gtsam/slam/PriorFactor.h>
#include <gtsam/slam/BetweenFactor.h>
#include <gtsam/slam/ProjectionFactor.h>
#include <gtsam/nonlinear/NonlinearFactorGraph.h>

/** GTSAM Optimizer **/
#include <gtsam/nonlinear/DoglegOptimizer.h>
#include <gtsam/nonlinear/GaussNewtonOptimizer.h>

/** GTSAM Marhinals **/
#include <gtsam/nonlinear/Marginals.h>

/** Values to estimate **/
#include <gtsam/nonlinear/Values.h>

#include <envire_sam/Conversions.hpp>
#include <envire_sam/Filters.hpp>
#include <envire_sam/Features.hpp>

namespace envire { namespace sam
{

    class PoseItem: public envire::core::Item<base::Pose>
    {
    };

    /** PCL TYPES **/
    typedef pcl::PointXYZRGB PointType;
    typedef pcl::PointCloud<PointType> PCLPointCloud;
    typedef typename PCLPointCloud::Ptr PCLPointCloudPtr;

    /**
     * A calss to perform SAM using PCL and Envire
     */
    class ESAM
    {

    private:

        /** Keys to identify poses and landmarks **/
        char pose_key, landmark_key;

        unsigned long int pose_idx, landmark_idx;

        /** The environment in a graph structure **/
        envire::core::TransformGraph _transform_graph;

        /** Factor graph **/
        gtsam::NonlinearFactorGraph _factor_graph;

        /** Optimzation **/
        gtsam::GaussNewtonParams parameters;

        /** Marginals in the estimation **/
        boost::shared_ptr<gtsam::Marginals> marginals;

        /** Values estimates **/
        gtsam::Values estimates_values;

        /** Input point cloud in pcl **/
        PCLPointCloudPtr pcl_point_cloud_in;

    public:

        /** Constructors
         */
        ESAM();
        ESAM(const ::base::Pose &pose, const ::base::Matrix6d &cov_pose, const char pose_key, const char landmark_key);
        ESAM(const ::base::Pose &pose, const ::base::Vector6d &var_pose, const char pose_key, const char landmark_key);

        ~ESAM();

        void insertFactor(const char key1, const unsigned long int &idx1,
                 const char key2, const unsigned long int &idx2,
                 const base::Time &time, const ::base::Pose &delta_pose,
                 const ::base::Vector6d &var_delta_pose);

        void insertFactor(const char key1, const unsigned long int &idx1,
                const char key2, const unsigned long int &idx2,
                const base::Time &time, const ::base::Pose &delta_pose,
                const ::base::Matrix6d &cov_delta_pose);

        void addFactor(const base::Time &time, const ::base::Pose &delta_pose, const ::base::Vector6d &var_delta_pose);

        void addFactor(const base::Time &time, const ::base::Pose &delta_pose, const ::base::Matrix6d &cov_delta_pose);
        
        void insertValue(const char key, const unsigned long int &idx, const ::base::Pose &pose);

        void optimize();

        void printMarginals();

        inline gtsam::NonlinearFactorGraph& factor_graph() { return this->_factor_graph; };

        void printFactorGraph(const std::string &title);

        void graphViz(const std::string &filename);
    };

}}
#endif

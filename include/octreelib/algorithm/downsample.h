/*
 * Software License Agreement (BSD License)
 *
 *  Copyright (c) 2011, University of Bonn, Computer Science Institute VI
 *  Author: Joerg Stueckler, 4/2011
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above
 *     copyright notice, this list of conditions and the following
 *     disclaimer in the documentation and/or other materials provided
 *     with the distribution.
 *   * Neither the name of University of Bonn, Computer Science Institute 
 *     VI nor the names of its contributors may be used to endorse or 
 *     promote products derived from this software without specific 
 *     prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 *  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 *  COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 *  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 *  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 *  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 *  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 *  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 *
 */

#ifndef __DOWNSAMPLE_OCTREE_H__
#define __DOWNSAMPLE_OCTREE_H__

#include <map>
#include <list>
#include <vector>

#include <octreelib/spatialaggregate/octree.h>


namespace algorithm {
	
	template< typename CoordType, typename ValueType >
	class OcTreeSamplingMap : public std::map< unsigned int, std::list< spatialaggregate::OcTreeNode< CoordType, ValueType >* > > {
	public:
//		typedef std::map< unsigned int, std::list< spatialaggregate::OcTreeNode< CoordType, ValueType >* > > super_type;
//		typedef super_type::iterator iterator;
//		typedef super_type::const_iterator const_iterator;
		OcTreeSamplingMap() {}
		~OcTreeSamplingMap() {}

	public:
   	    EIGEN_MAKE_ALIGNED_OPERATOR_NEW;
	};

	// downToDepth: include a leaf at depth in lists from 0 to depth
	template< typename CoordType, typename ValueType >
	OcTreeSamplingMap< CoordType, ValueType > downsampleOcTree( const spatialaggregate::OcTree< CoordType, ValueType >& tree, bool downToMaxDepth = false, unsigned int maxDepth = 0, bool branchMaxDepthNodes = true ) {

		algorithm::OcTreeSamplingMap< CoordType, ValueType > samplingMap;

		std::list< spatialaggregate::OcTreeNode< CoordType, ValueType >* > openList;
		openList.push_back( tree.root_ );

		while( !openList.empty() ) {

			spatialaggregate::OcTreeNode< CoordType, ValueType >* node = openList.front();

			if( node->type_ == spatialaggregate::OCTREE_BRANCHING_NODE || ( branchMaxDepthNodes && node->type_ == spatialaggregate::OCTREE_MAX_DEPTH_BRANCHING_NODE ) ) {

				samplingMap[ node->depth_ ].push_back( node );

				for( unsigned int i = 0; i < 8; i++ ) {
					if( node->children_[i] )
						openList.push_back( node->children_[i] );
				}

			}
			else {

				// handle leaf node

				if( downToMaxDepth ) {
					for( int i = node->depth_; i <= maxDepth; i++ )
						samplingMap[ i ].push_back( node );
				}
				else
					samplingMap[ node->depth_ ].push_back( node );

			}

			openList.pop_front();

		}

		return samplingMap;

	}



	template< typename CoordType, typename ValueType >
	class OcTreeSamplingVectorMap : public std::map< unsigned int, std::vector< spatialaggregate::OcTreeNode< CoordType, ValueType >* > > {
	public:
		OcTreeSamplingVectorMap() {}
		~OcTreeSamplingVectorMap() {}

	public:
   	    EIGEN_MAKE_ALIGNED_OPERATOR_NEW;
	};

	// downToDepth: include a leaf at depth in lists from 0 to depth
	template< typename CoordType, typename ValueType >
	OcTreeSamplingVectorMap< CoordType, ValueType > downsampleVectorOcTree( const spatialaggregate::OcTree< CoordType, ValueType >& tree, bool downToMaxDepth = false, unsigned int maxDepth = 0, bool branchMaxDepthNodes = true ) {

		typedef OcTreeSamplingMap< CoordType, ValueType > SamplingMap;
		//typedef SamplingMap::iterator samplingmap_iterator;

		SamplingMap samplingMap = downsampleOcTree( tree, downToMaxDepth, maxDepth, branchMaxDepthNodes );
		OcTreeSamplingVectorMap< CoordType, ValueType > samplingVectorMap;

		typename SamplingMap::iterator it = samplingMap.begin();

		while( it != samplingMap.end() ) {

			samplingVectorMap[ it->first ].assign( it->second.begin(), it->second.end() );
			++it;

		}

		return samplingVectorMap;

	}

};

#include <octreelib/algorithm/impl/downsample.hpp>

#endif //__DOWNSAMPLE_OCTREE_H__



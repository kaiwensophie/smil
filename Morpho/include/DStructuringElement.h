/*
 * Copyright (c) 2011, Matthieu FAESSEL and ARMINES
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of Matthieu FAESSEL, or ARMINES nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDERS AND CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */


#ifndef _STR_ELT_HPP
#define _STR_ELT_HPP

/**
 * \defgroup StrElt Structuring Elements
 * \ingroup Morpho
 * @{
 */

struct Point
{
  int x;
  int y;
  int z;
};

enum seType { SE_Generic, SE_Hex, SE_Squ };

/**
 * Base structuring element
 */
class StrElt : public BaseObject
{
  public:
    StrElt(UINT s=1)
      : BaseObject("StrElt"),
	seT(SE_Generic), size(s) 
    {
    }
    ~StrElt()
    {
    }
    StrElt(const StrElt &rhs)
      : BaseObject(rhs)
    {
	this->clone(rhs);
    }
    StrElt& operator=(const StrElt &rhs)
    {
	this->clone(rhs);
	return *this;
    }
    
    void clone(const StrElt &rhs)
    {
	this->seT = rhs.seT;
	this->size = rhs.size;
	this->odd = rhs.odd;
	this->points = rhs.points;
    }
   
    //! List of neighbor points
    vector<Point> points;
    
    inline void addPoint(int x, int y, int z=0)
    {
	Point p;
	p.x = x;
	p.y = y;
	p.z = z;
	points.push_back(p);
    }
    inline const StrElt operator()(int s=1)
    {
	StrElt se(*this);
	se.size = s;
	return se;
    }
    bool odd;
    seType seT;
    UINT size;
    virtual seType getType() const { return seT; }
    
    virtual void printSelf(ostream &os=std::cout)
    {
	os << "Structuring Element" << endl;
	os << "Size: " << size << endl;
	int ptNbr = points.size();
	os << "Point Nbr: " << ptNbr << endl;
	if (ptNbr)
	  for (int i=0;i<ptNbr;i++)
	    os << "#" << i+1 << ": (" << points[i].x << "," << points[i].y << "," << points[i].z << ")" << endl;
	  
    }
};

inline void operator << (ostream &os, StrElt &se)
{
    se.printSelf(os);
}

/**
 * Hexagonal structuring element.
 * 
 * Points :
 * 
 * <table>
 *   <tr>  <th>4</th> <th>3</th> <th>x</th>  </tr>
 *   <tr>  <th>5</th> <th>1</th> <th>2</th>  </tr>
 *   <tr>  <th>6</th> <th>7</th> <th>x</th>  </tr>
 * </table>
 * 
 */

class HexSE : public StrElt
{
  public:
    HexSE(UINT s=1)
    {
	className = "HexSE";
	seT = SE_Hex;
	size = s;
	odd = true;
	addPoint(0,0);		// 1
	addPoint(1,0);		// 2
	addPoint(0,-1);		// 3
	addPoint(-1,-1);	// 4
	addPoint(-1,0);		// 5
	addPoint(-1,1);		// 6
	addPoint(0,1);		// 7
    }
};

/**
 * Hexagonal structuring element without center point.
 * 
 * Points :
 * 
 * <table>
 *   <tr>  <th>3</th> <th>2</th> <th>x</th>  </tr>
 *   <tr>  <th>4</th> <th>x</th> <th>1</th>  </tr>
 *   <tr>  <th>5</th> <th>6</th> <th>x</th>  </tr>
 * </table>
 * 
 */

class HexSE0 : public StrElt
{
  public:
    HexSE0(UINT s=1) 
    {
	className = "HexSE0";
// 	seT = SE_Hex;
	size = s;
	odd = true;
	addPoint(1,0);		// 1
	addPoint(0,-1);		// 2
	addPoint(-1,-1);	// 3
	addPoint(-1,0);		// 4
	addPoint(-1,1);		// 5
	addPoint(0,1);		// 6
    }
};


/**
 * Square structuring element.
 * 
 * Points :
 * 
 * <table>
 *   <tr>  <th>5</th> <th>4</th> <th>3</th>  </tr>
 *   <tr>  <th>6</th> <th>1</th> <th>2</th>  </tr>
 *   <tr>  <th>7</th> <th>8</th> <th>9</th>  </tr>
 * </table>
 * 
 */

class SquSE : public StrElt
{
  public:
    SquSE(UINT s=1) : StrElt(s)
    {
	className = "SquSE";
	seT = SE_Squ;
	odd = false;
	addPoint(0,0); 	// 1
	addPoint(1,0);	// 2
	addPoint(1,-1);	// 3
	addPoint(0,-1);	// 4
	addPoint(-1,-1);	// 5
	addPoint(-1,0);	// 6
	addPoint(-1,1);// 7
	addPoint(0,1);	// 8
	addPoint(1,1);	// 9
    }
};

/**
 * Square structuring element without center point.
 * 
 * Points :
 * 
 * <table>
 *   <tr>  <th>4</th> <th>3</th> <th>2</th>  </tr>
 *   <tr>  <th>5</th> <th>x</th> <th>1</th>  </tr>
 *   <tr>  <th>6</th> <th>7</th> <th>8</th>  </tr>
 * </table>
 * 
 */

class SquSE0 : public StrElt
{
  public:
    typedef StrElt parentClass;
    SquSE0(UINT s=1) : StrElt(s)
    {
	className = "SquSE0";
// 	seT = stSquSE;
	odd = false;
	addPoint(1,0);		// 1
	addPoint(1,-1);		// 2
	addPoint(0,-1);		// 3
	addPoint(-1,-1);	// 4
	addPoint(-1,0);		// 5
	addPoint(-1,1);		// 6
	addPoint(0,1);		// 7
	addPoint(1,1);		// 8
    }
};


/**
 * 3D Cubic structuring element.
 * 
 * Points :
 * 
 * z=-1
 * <table>
 *   <tr>  <th>14</th> <th>13</th> <th>12</th>  </tr>
 *   <tr>  <th>15</th> <th>10</th> <th>11</th>  </tr>
 *   <tr>  <th>16</th> <th>17</th> <th>18</th>  </tr>
 * </table>
 * z=0
 * <table>
 *   <tr>  <th>5</th> <th>4</th> <th>3</th>  </tr>
 *   <tr>  <th>6</th> <th>1</th> <th>2</th>  </tr>
 *   <tr>  <th>7</th> <th>8</th> <th>9</th>  </tr>
 * </table>
 * z=1
 * <table>
 *   <tr>  <th>23</th> <th>22</th> <th>21</th>  </tr>
 *   <tr>  <th>24</th> <th>19</th> <th>20</th>  </tr>
 *   <tr>  <th>25</th> <th>26</th> <th>27</th>  </tr>
 * </table>
 * 
 */
class CubeSE : public StrElt
{
  public:
    CubeSE(UINT s=1) : StrElt(s)
    {
	className = "CubeSE";
	odd = false;
	int zList[] = { 0, -1, 1 };
	for (int i=0;i<3;i++)
	{
	    int z = zList[i];
	    addPoint(0,0,z);	// 1
	    addPoint(1,0,z);	// 2
	    addPoint(1,-1,z);	// 3
	    addPoint(0,-1,z);	// 4
	    addPoint(-1,-1,z);	// 5
	    addPoint(-1,0,z);	// 6
	    addPoint(-1,1,z);	// 7
	    addPoint(0,1,z);	// 8
	    addPoint(1,1,z);	// 9
	}
    }
};

// Shortcuts
HexSE hSE(UINT s=1) { return HexSE(s); }
HexSE0 hSE0(UINT s=1) { return HexSE0(s); }
SquSE sSE(UINT s=1) { return SquSE(s); }
SquSE0 sSE0(UINT s=1) { return SquSE0(s); }
CubeSE cSE(UINT s=1) { return CubeSE(s); }

StrElt DEFAULT_SE = HexSE();

StrElt& getDefaultSE()
{
    return DEFAULT_SE;
}

void setDefaultSE(const StrElt &se)
{
    DEFAULT_SE = se;
}

// #define DEFAULT_SE sSE

/** @} */

#endif

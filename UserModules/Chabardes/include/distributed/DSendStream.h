#ifndef _DSENDSTREAM_H_
#define _DSENDSTREAM_H_

namespace smil {
    template <class T>
    class SendStream {
        public:
            virtual RES_T read_at (const unsigned int num, Chunk<T> &c) {
                cout << "Don't know how to read." << endl;
            }
    };

    template <class T>
    class SendArrayStream : public SendStream<T> {
        public:
            RES_T read_at (const unsigned int num, Chunk<T> &c) {
                unsigned int x,y,z,s_x,s_y,s_z,w_x,w_y,w_z,w_s_x,w_s_y,w_s_z;
                w_z = num/(chunks_per_dim[0]*chunks_per_dim[1]);
                w_y = num/chunks_per_dim[0]-w_z*chunks_per_dim[1];
                w_x = num-w_y*chunks_per_dim[0]-w_z*chunks_per_dim[1]*chunks_per_dim[0];
                w_s_x = (w_x==chunks_per_dim[0]-1) ? size[0] - w_x*size[0]/chunks_per_dim[0] : size[0]/chunks_per_dim[0] ;
                w_x *= size[0]/chunks_per_dim[0];
                w_s_y = (w_x==chunks_per_dim[1]-1) ? size[1] - w_x*size[1]/chunks_per_dim[1] : size[1]/chunks_per_dim[1];
                w_y *= size[1]/chunks_per_dim[1];
                w_s_z = (w_x==chunks_per_dim[2]-1) ? size[2] - w_x*size[2]/chunks_per_dim[2] : size[2]/chunks_per_dim[2];
                w_z *= size[2]/chunks_per_dim[2];
                x = (w_x > intersect_width) ? w_x-intersect_width : 0;
                y = (w_y > intersect_width) ? w_y-intersect_width : 0;
                z = (w_z > intersect_width) ? w_z-intersect_width : 0;
                s_x = (w_x+w_s_x+intersect_width < size[0]) ? (w_x-x)+w_s_x+intersect_width : size[0]-x;
                s_y = (w_y+w_s_y+intersect_width < size[1]) ? (w_y-y)+w_s_y+intersect_width : size[1]-y;
                s_z = (w_z+w_s_z+intersect_width < size[2]) ? (w_z-z)+w_s_z+intersect_width : size[2]-z;

                c.createFromArray (
                            data,
                            size[0],
                            size[1],
                            x, y, z,
                            s_x, s_y, s_z,
                            w_x, w_y, w_z,
                            w_s_x, w_s_y, w_s_z
                        );
                return RES_OK;
            }
            unsigned int intersect_width;
            unsigned int chunks_per_dim[3];
            unsigned int nbr_chunks;
            unsigned long chunk_len;
            unsigned int size[3];
            T* data;
    };

}

#endif
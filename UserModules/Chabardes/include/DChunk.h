#ifndef _DCHUNK_H_
#define _DCHUNK_H_

namespace smil {

    template <class T>
    class Chunk {
        public:
            Chunk () : is_initialized (false) { }
            ~Chunk () {
            }
            RES_T setMemorySpace (void* ptr, const unsigned long _sent_size, MPI_Datatype dt) {
                sent_size = _sent_size;
                if (ptr == NULL) { return RES_ERR_BAD_ALLOCATION; }
                is_initialized = true; 
                datatype = dt;
                rawData = ptr;
                size = (unsigned int*)ptr;
                offset = (unsigned int*)ptr + 3;
                w_size = (unsigned int*)ptr + 6;
                w_offset = (unsigned int*)ptr + 9;
                data = (T*)((unsigned int*)ptr + 12);
            }
            RES_T createFromArray (
                    const T* dataIn,
                    const unsigned int &sd_x, 
                    const unsigned int &sd_y,
                    const unsigned int &o_x,
                    const unsigned int &o_y,
                    const unsigned int &o_z,
                    const unsigned int &sc_x,
                    const unsigned int &sc_y,
                    const unsigned int &sc_z,
                    const unsigned int &w_o_x,
                    const unsigned int &w_o_y,
                    const unsigned int &w_o_z,
                    const unsigned int &w_s_x,
                    const unsigned int &w_s_y,
                    const unsigned int &w_s_z) {
                ASSERT (is_initialized);
                size[0] = sc_x; size[1] = sc_y; size[2] = sc_z;
                offset[0] = o_x; offset[1] = o_y; offset[2] = o_z; 
                w_size[0] = w_s_x; w_size[1] = w_s_y; w_size[2] = w_s_z;
                w_offset[0] = w_o_x; w_offset[1] = w_o_y; w_offset[2] = w_o_z;              


                for (int k=0; k<sc_z; ++k) {
                    for (int j=0; j<sc_y; ++j) { 
                        memcpy (data + j * sc_x + k * sc_x * sc_y, 
                                    dataIn + o_x + (o_y+j) * sd_x + (o_z+k) * sd_x * sd_y,
                                    sc_x*sizeof(T));
                    }
                }
                return RES_OK;
            }
            RES_T storeToArray (const int &sd_x, const int &sd_y, T* dataOut) {
                ASSERT (is_initialized && dataOut != NULL);
                for (int k=0; k<w_size[2]; ++k) {
                    for (int j=0; j<w_size[1]; ++j) {
                            memcpy (dataOut + w_offset[0] +
                                (w_offset[1]+j) * sd_x + 
                                (w_offset[2]+k) * sd_x * sd_y,
                                    data + w_offset[0]-offset[0] + 
                                (j+w_offset[1]-offset[1]) * size[0] + 
                                (k+w_offset[2]-offset[2]) * size[0] * size[1], 
                                    w_size[0]*sizeof(T)); 
                    }
                }
                return RES_OK;
            } 
            RES_T storeToImage (Image<T> &imOut) {
                size_t s[3];
                imOut.getSize(s);

                if (storeToArray (s[0], s[1], imOut.getPixels()) != RES_OK) return RES_ERR;
                return RES_OK;
            }
            const int getSize (const unsigned char &dimension) const {
                ASSERT (dimension < 3);
                return size[dimension];
            }
            int* getSize () {
                return size;
            }
            const int getOffset (const unsigned char &dimension) const {
                ASSERT (dimension < 3);
                return offset[dimension];
            }
            int* getOffset () {
                return offset;
            }
            const int getWrittenSize (const unsigned char &dimension) const {
                ASSERT (dimension < 3);
                return w_size[dimension];
            }
            int* getWrittenSize () {
                return w_size;
            }
            const int getWrittenOffset (const unsigned char &dimension) const {
                ASSERT (dimension < 3);
                return w_offset (dimension);
            }
            int* getWrittenOffset () {
                return w_offset;
            }
            int getRelativeWrittenOffset (const unsigned char &dimension) const {
                return w_offset[dimension] - offset[dimension];
            }
            T* getData () {
                return data;
            }
            void print (bool print_data = false) {
                cout << "#### size : [" << size[0] << ", " << size[1] << ", " << size[2] << "], offset : [" << offset[0] << ", " << offset[1] << ", " << offset[2] << "], w_size : [" << w_size[0] << ", " << w_size[1] << ", " << w_size[2] << "], offset : [" << w_offset[0] << ", " << w_offset[1] << ", " << w_offset[2] << "] ####" << endl;
                if (!print_data) return;
                for (int i=0; i<size[2]; ++i) {
                    for (int j=0; j<size[1]; ++j) {
                        for (int k=0; k<size[0]; ++k)
                            cout << (int) data[k+j*size[0]+i*size[0]*size[1]] << ' ';
                        cout << endl;
                    }
                    cout << endl;
                }
                cout << endl; 
            }
            bool isInitialized () {
                return is_initialized;
            }
            RES_T send (const int dest, const int tag, const MPI_Comm &comm) {
                ASSERT (is_initialized);
                MPI_Send (rawData, 1, datatype, dest, tag, comm);
            } 
            RES_T recv (const int root, const int tag, const MPI_Comm &comm, MPI_Status* status) {
                ASSERT (is_initialized);
                MPI_Recv (rawData, 1, datatype, root, tag, comm, status);
            }
        private:
            bool is_initialized;
            unsigned long sent_size;
            // PACKET CONTENT
            unsigned int* size;
            unsigned int* offset;
            unsigned int* w_size;
            unsigned int* w_offset;
            T* data;
            // END OF PACKET CONTENT
            // Raw pointer to the packet
            void* rawData;
            MPI_Datatype datatype;
    };

}

#endif

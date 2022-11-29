#include "H5Cpp.h"

int main(void) {
    H5::Exception::dontPrint();

    H5::H5File *file = new H5::H5File("test_file.h5", H5F_ACC_TRUNC);

    int fillvalue = 0;
    H5::DSetCreatPropList plist;
    plist.setFillValue(H5::PredType::NATIVE_INT, &fillvalue);

    hsize_t fdim[] = {2, 3, 4};
    H5::DataSpace fspace(3, fdim);

    H5::DataSet *dataset = new H5::DataSet(file->createDataSet("data",
                                           H5::PredType::NATIVE_INT,
                                           fspace, plist));

    int data[2][3][4] = { { {  1,  2,  3,  4 },
                            {  5,  6,  7,  8 },
                            {  9, 10, 11, 12 } },
                          { { 13, 14, 15, 16 },
                            { 17, 18, 19, 20 },
                            { 21, 22, 23, 24 } } };
    dataset->write(data, H5::PredType::NATIVE_INT);

    delete dataset;
    delete file;

    return 0;
}


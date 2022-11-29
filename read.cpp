#include <iostream>
#include <vector>
#include "H5Cpp.h"

std::vector<std::vector<int> > extractData(const H5::DataSet &dataset,
                                           size_t tubeIndex, size_t pixelIndex,
                                           size_t channelIndex) {

    // Get data space
    H5::DataSpace dataspace = dataset.getSpace();

    // get dataset dimensions
    int nDims = dataspace.getSimpleExtentNdims();
    hsize_t dims[nDims];
    int ndims = dataspace.getSimpleExtentDims(dims, NULL);

    // data dimensions
    size_t nPixel = dims[pixelIndex];
    size_t nTube = dims[tubeIndex];
    size_t nChannel = dims[channelIndex];

    // fill the workspace
    std::vector<std::vector<int> > workspace;
    for (size_t i = 0; i < nTube; i++) {
        for (size_t j = 0; j < nPixel; j++) {
            // offset of the slice in the dataset
            // this correspond to one spectrum: all channel of a specific pixel
            hsize_t offset_out[3];
            offset_out[tubeIndex] = i;
            offset_out[pixelIndex] = j;
            offset_out[channelIndex] = 0;

            // dimension of the slice in the dataset
            // One specific pixel, all channels
            hsize_t count_out[3];
            count_out[tubeIndex] = 1;
            count_out[pixelIndex] = 1;
            count_out[channelIndex] = nChannel;

            // select the hyperslab
            dataspace.selectHyperslab(H5S_SELECT_SET, count_out, offset_out);

            // create the dataspace of the spectrum
            hsize_t dimsm[1] = {nChannel};
            H5::DataSpace memspace(1, dimsm);

            // read the data
            std::vector<int> spectrum(nChannel);
            dataset.read(spectrum.data(), H5::PredType::NATIVE_INT, memspace,
                         dataspace);

            // add the spectrum to the workspace
            workspace.push_back(spectrum);
        }
    }

    return workspace;
}

int main(void) {

    H5::Exception::dontPrint();

    H5::H5File  file("test_file.h5", H5F_ACC_RDONLY);
    H5::DataSet dataset = file.openDataSet("data");

    H5::DataSpace dataspace = dataset.getSpace();
    int nDims = dataspace.getSimpleExtentNdims();
    hsize_t dims[nDims];
    int ndims = dataspace.getSimpleExtentDims(dims, NULL);

    std::cout << "Dataset: " << nDims << " dimensions" << std::endl;
    for (int i = 0; i < nDims; i++)
        std::cout << "  " << i << ": " << dims[i] << " values" << std::endl;


    std::cout << "First case: 0,1,2" << std::endl;
    std::vector<std::vector<int> > workspace1 = extractData(dataset, 0, 1, 2);
    for (const auto &spectrum: workspace1) {
        for (auto value : spectrum) {
            std::cout << value << " ";
        }
        std::cout << std::endl;
    }

    std::cout << "Second case: 1,2,0" << std::endl;
    std::vector<std::vector<int> > workspace2 = extractData(dataset, 1, 2, 0);
    for (const auto &spectrum: workspace2) {
        for (auto value : spectrum) {
            std::cout << value << " ";
        }
        std::cout << std::endl;
    }

    return 0;
}


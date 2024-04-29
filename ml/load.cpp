#include <torch/script.h> // One-stop header.
#include <iostream>
#include <memory>
#include <vector>

int main() {
    // Step 1: Load the TorchScript model
    torch::jit::script::Module module;
    try {
        // Deserialize the ScriptModule from a file using torch::jit::load().
        module = torch::jit::load("model.pt");
    }
    catch (const c10::Error& e) {
        std::cerr << "Error loading the model.\n";
        return -1;
    }

    std::cout << "Model loaded successfully.\n";

    // Set the module in evaluation mode
    module.eval();

    // Step 2: Create a vector of inputs
    // Example input data: body_1_x = 207.0, body_1_y = 200.0, body_2_x = 249.0, body_2_y = 200.0
    std::vector<torch::jit::IValue> inputs;
    inputs.push_back(torch::tensor({207.0, 200.0, 249.0, 200.0}).reshape({1, 4}));

    // Step 3: Execute the model and use torch::Tensor to hold the output
    torch::Tensor output;
    try {
        output = module.forward(inputs).toTensor();
    }
    catch (const std::exception& e) {
        std::cerr << "Error during model forward pass.\n";
        return -1;
    }

    // Convert torch::Tensor to std::vector<float>
    std::vector<float> output_vector(output.data_ptr<float>(), output.data_ptr<float>() + output.numel());

    for (float value : output_vector) {
        std::cout << value << " ";
    }
    std::cout << std::endl;

    return 0;
}
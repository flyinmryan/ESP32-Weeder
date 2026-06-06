import os

def convert_tflite_to_c_header(tflite_path, header_path):
    with open(tflite_path, 'rb') as f:
        data = f.read()
    
    with open(header_path, 'w') as f:
        f.write('#ifndef MODEL_DATA_H\n')
        f.write('#define MODEL_DATA_H\n\n')
        f.write(f'const unsigned char g_model_data[] = {{\n')
        
        for i, b in enumerate(data):
            f.write(f'0x{b:02x}, ')
            if (i + 1) % 12 == 0:
                f.write('\n  ')
        
        f.write('\n};\n\n')
        f.write(f'const unsigned int g_model_data_len = {len(data)};\n\n')
        f.write('#endif // MODEL_DATA_H\n')

if __name__ == '__main__':
    convert_tflite_to_c_header('model_int8.tflite', 'esp32_firmware/include/model_data.h')
    print("Model converted successfully to esp32_firmware/include/model_data.h")

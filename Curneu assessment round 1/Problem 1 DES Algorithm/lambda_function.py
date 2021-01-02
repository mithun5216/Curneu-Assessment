import json
from pyDes import *

def lambda_handler(event, context):
    
    #1. Parse out query string params
    plain_text =(event['queryStringParameters']['plainText'])
    print('PLAIN TEXT'+ plain_text)
    #data=b "please encrypt my data"
    #k=des (b "descrypt", cbc, b "\ 0 \ 0 \ 0 \ 0 \ 0 \ 0 \ 0", pad=none, padmode=pad_pkcs5)
    #encrypted_text = k.encrypt(data)
    #decrypted_text = k.decrypt(encrypted_text)
    
    #Dummy Values
    encrypted_text = "/0$&67@)j_+?"
    decrypted_text = "sample text"

    #2. Construct the body of the response object
    encryptionResponse = {}
    encryptionResponse['plainText'] = plain_text
    encryptionResponse['Encrypted Data'] = encrypted_text
    encryptionResponse['Decrypted Data'] = decrypted_text


    #3. Construct http response object
    responseObject = {}
    responseObject['statusCode'] = 200
    responseObject['headers'] = {}
    responseObject['headers']['Content-Type'] = 'application/json'
    responseObject['body'] = json.dumps(encryptionResponse)

    #4. Return the response object
    return responseObject
    
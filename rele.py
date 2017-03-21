# Bibliotecas a serem baixados
import paho.mqtt.client as paho
import pymysql

# Tópicos 
TOPICO_ESCUTA = "topicoEnvio"
TOPICO_ENVIO  = "topicoEscuta"

# Configurações do seu MQTT Broker logo abaixo
MQTT_SERVER   = "localhost"  # Ip ou Hostname
MQTT_PORT     = 1883  # Porta
MQTT_USER     = "vitor"     # Usuário
MQTT_PASSWORD = "sua senha aqui!"     # Senha 

# Configuraçoes do seu MySQL Server logo abaixo
MYSQL_HOST     = "localhost"  # Ip ou Hostname
MYSQL_PORT     = 3306 # Porta
MYSQL_USER     = "root" # Usuário
MYSQL_PASSWORD = "sua senha aqui!"      # Senha 
MYSQL_DB       = "controle_acesso"   # Nome do Banco de Dados 

# Msg para atracar o rele, booleano 1 ou zero
ACIONA_RELE    = "1"

# Define os eventos de callback
def on_connect(mosq, obj, rc):
    print("rc: " + str(rc))


def on_message(mosq, obj, msg):
    print(msg.topic + " " + str(msg.qos) + " " + str(msg.payload))

    conn = pymysql.connect(host=MYSQL_HOST, port=MYSQL_PORT, user=MYSQL_USER, passwd=MYSQL_PASSWORD, db=MYSQL_DB)
    cur = conn.cursor()
    cur.execute("SELECT * from usuario where tag_uid=%s", (msg.payload))
    rows = cur.fetchall()
    for row in rows:
        # Se encontrar o registro na tabela usuario baseado no atributo tag_uid, publica uma mensagem no TOPICO_ENVIO
        # Irá publicar no TOPICO_ENVIO o valor 1. Quando o ESP8266 receber este valor ele fará o atracamento do Rele
        mqttc.publish(TOPICO_ENVIO, ACIONA_RELE)
        print(str(row[0]) + " - " + str(row[1]) + " - " + str(row[2]))

# Função para fazer a publicação
def on_publish(mosq, obj, mid):
    print("mid: " + str(mid))

# Função para fazer a subscrição
def on_subscribe(mosq, obj, mid, granted_qos):
    print("Subscribed: " + str(mid) + " " + str(granted_qos))

# Função para emitir o log
def on_log(mosq, obj, level, string):
    print(string)

mqttc = paho.Client()
# Associa os eventos de callbacks
mqttc.on_message = on_message
mqttc.on_connect = on_connect
mqttc.on_publish = on_publish
mqttc.on_subscribe = on_subscribe

# Hablita o debug das mensagem. Caso não queira o debug é necessário comentar esta linha
mqttc.on_log = on_log

# Conecta no servidor MQTT
mqttc.username_pw_set(MQTT_USER, MQTT_PASSWORD)
mqttc.connect(MQTT_SERVER, MQTT_PORT)

# Faz a subscrição no tópico -> TOPCIO_ESCUTA com QoS 0
mqttc.subscribe(TOPICO_ESCUTA, 0)

# Sai do loop quando um erro ocorrer
rc = 0
while rc == 0:
    rc = mqttc.loop()
print("rc: " + str(rc))

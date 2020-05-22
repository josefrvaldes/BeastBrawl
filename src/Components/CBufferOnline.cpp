/**
 * Beast Brawl
 * Game created as a final project of the Multimedia Engineering Degree in the University of Alicante.
 * Made by Clover Games Studio, with members 
 * Carlos de la Fuente Torres delafuentetorresc@gmail.com,
 * Antonio Jose Martinez Garcia https://www.linkedin.com/in/antonio-jose-martinez-garcia/,
 * Jesús Mas Carretero jmasc03@gmail.com, 
 * Judith Mula Molina https://www.linkedin.com/in/judith-mm-18099215a/, 
 * Rubén Rubio Martínez https://www.linkedin.com/in/rub%C3%A9n-rubio-mart%C3%ADnez-938700131/, 
 * and Jose Valdés Sirvent https://www.linkedin.com/in/jose-f-valdés-sirvent-6058b5a5/ github -> josefrvaldes
 * 
 * 
 * @author Clover Games Studio
 * 
 */
 
 
 #include "CBufferOnline.h"

CBufferOnline::CBufferOnline() {
    m_compType = CompType::BufferOnline;
}

CBufferOnline::~CBufferOnline() {
}

/**
 * Esta función recibe un elemento nuevo y lo inserta donde corresponda manteniendo el orden por tiempo. Además, elimina todos los anteriores que hubiera por tiempo.
 */
void CBufferOnline::InsertNewReceivedOnline(int64_t time, int64_t timeSent, vector<Constants::InputTypes> inputs, float speed_, float wheelRotation_, float skidDeg_, float skidRotation_) {
    BuffElement buffElem(time, timeSent, inputs, speed_, wheelRotation_, skidDeg_, skidRotation_);
    if(elems.empty()) {
        elems.push_back(buffElem);
        return;
    }

    std::list<BuffElement>::iterator it;
    // recorremos los elementos, están ordenados, hay que asegurarse de que el actual lo insertamos ordenado también. Además
    // tenemos que borrar todos los elementos que sean más antiguos que la hora en la que se envió el que acabamos de recibir
    // la idea es dejar la lista con el primer elemento que sea el que acabamos de recibir, y el resto son los calculados que se han ejecutado automáticamente
    for(it = elems.begin(); it != elems.end();)  {
        // si encontramos un elemento más antiguo que el recién recibido, lo eliminamos
        if(it->time < buffElem.timeSent) {
            it = elems.erase(it);

            // en cuanto encontramos un elemento posterior a cuando se envió el que acabamos de recibir, justo ahí insertamos y salimos
        } else {
            elems.insert(it, buffElem);
            break;
        }
    }

    // en teoría al acabar esto, tiene que quedar el primer elemento con receivedForReal = true, porque es el último online que hemos recibido
    // y el resto de elementos son elementos que se han calculado después de que ese se enviara en la máquina local que serán los que tendremos 
    // como referencia para corregir después
}

/**
 * Esta función recibe un elemento nuevo y lo inserta donde corresponda manteniendo el orden por tiempo. Además, elimina todos los anteriores que hubiera por tiempo.
 */
void CBufferOnline::InsertNewCalculated(int64_t time_, vec3 pos_, vec3 rot_, float speed_, float wheelRotation_, float skidDeg_, float skidRotation_) {
    BuffElement buffElem(time_, pos_, rot_, speed_, wheelRotation_, skidDeg_, skidRotation_);
    elems.push_back(buffElem);

    // si hay demasiados elementos porque por ejemplo se puede haber perdido un paquete, guardamos el último al final pero eliminamos el primero
    if(elems.size() > 32) {
        elems.pop_front();
    }
}
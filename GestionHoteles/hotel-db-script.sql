-- Eliminar tablas si existen para evitar errores
DROP TABLE IF EXISTS registros_actividad;
DROP TABLE IF EXISTS facturacion;
DROP TABLE IF EXISTS reservas;
DROP TABLE IF EXISTS habitaciones;
DROP TABLE IF EXISTS empleados;
DROP TABLE IF EXISTS clientes;
DROP TABLE IF EXISTS usuarios;

-- Crear tabla de Usuarios
CREATE TABLE usuarios (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    nombre TEXT NOT NULL,
    rol TEXT NOT NULL CHECK (rol IN ('Administrador', 'Recepcionista', 'Limpieza', 'Mantenimiento')),
    nombre_usuario TEXT UNIQUE NOT NULL,
    contraseña TEXT NOT NULL
);

-- Crear tabla de Clientes
CREATE TABLE clientes (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    nombre TEXT NOT NULL,
    dni TEXT NOT NULL,
    telefono TEXT,
    email TEXT,
    direccion TEXT,
    fecha_registro DATE DEFAULT CURRENT_DATE
);

-- Crear tabla de Empleados
CREATE TABLE empleados (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    id_usuario INTEGER,
    nombre TEXT NOT NULL,
    cargo TEXT NOT NULL,
    turno TEXT CHECK (turno IN ('Mañana', 'Tarde', 'Noche')),
    salario REAL NOT NULL,
    fecha_contratacion DATE DEFAULT CURRENT_DATE,
    FOREIGN KEY (id_usuario) REFERENCES usuarios(id)
);

-- Crear tabla de Habitaciones
CREATE TABLE habitaciones (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    numero TEXT UNIQUE NOT NULL,
    tipo TEXT NOT NULL CHECK (tipo IN ('Individual', 'Doble', 'Suite', 'Familiar')),
    precio REAL NOT NULL,
    estado TEXT NOT NULL DEFAULT 'Disponible' CHECK (estado IN ('Disponible', 'Ocupada', 'Mantenimiento', 'Limpieza')),
    capacidad INTEGER NOT NULL,
    descripcion TEXT
);

-- Crear tabla de Reservas
CREATE TABLE reservas (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    id_cliente INTEGER NOT NULL,
    id_habitacion INTEGER NOT NULL,
    fecha_entrada DATE NOT NULL,
    fecha_salida DATE NOT NULL,
    estado TEXT NOT NULL DEFAULT 'Pendiente' CHECK (estado IN ('Pendiente', 'Confirmada', 'Cancelada', 'Completada')),
    monto REAL NOT NULL,
    observaciones TEXT,
    CHECK (fecha_salida > fecha_entrada),
    FOREIGN KEY (id_cliente) REFERENCES clientes(id),
    FOREIGN KEY (id_habitacion) REFERENCES habitaciones(id)
);

-- Crear tabla de Facturación
CREATE TABLE facturacion (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    id_reserva INTEGER NOT NULL,
    id_cliente INTEGER NOT NULL,
    monto REAL NOT NULL,
    metodo_pago TEXT NOT NULL CHECK (metodo_pago IN ('Efectivo', 'Tarjeta de Crédito', 'Tarjeta de Débito', 'Transferencia')),
    fecha DATE DEFAULT CURRENT_DATE,
    numero_factura TEXT UNIQUE NOT NULL,
    estado TEXT DEFAULT 'Pagada' CHECK (estado IN ('Pagada', 'Pendiente', 'Cancelada')),
    FOREIGN KEY (id_reserva) REFERENCES reservas(id),
    FOREIGN KEY (id_cliente) REFERENCES clientes(id)
);

-- Crear tabla de Registros de Actividad
CREATE TABLE registros_actividad (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    id_usuario INTEGER NOT NULL,
    fecha DATETIME DEFAULT CURRENT_TIMESTAMP,
    operacion TEXT NOT NULL,
    detalles TEXT,
    FOREIGN KEY (id_usuario) REFERENCES usuarios(id)
);

-- Insertar datos de prueba

-- Usuarios de prueba
INSERT INTO usuarios (nombre, rol, nombre_usuario, contraseña) VALUES 
('Juan Pérez', 'Administrador', 'admin', 'admin'),
('María López', 'Recepcionista', 'recepcion', '123'),
('Carlos Ruiz', 'Limpieza', 'limpieza', '123'),
('Ana Gómez', 'Mantenimiento', 'mantenimiento', '123');

-- Clientes de prueba
INSERT INTO clientes (nombre, dni, telefono, email, direccion) VALUES 
('Pedro Martínez', '12345678A', '666111222', 'pedro@mail.com', 'Calle Mayor 1'),
('Laura García', '87654321C', '666333444', 'laura@mail.com', 'Avd. Principal 23'),
('Miguel Fernández', '87654321B', '666555666', 'miguel@mail.com', 'Plaza Central 5'),
('Sofía Rodríguez', 'X1234567Z', '666777888', 'sofia@mail.com', 'Calle Nueva 11');

-- Empleados de prueba
INSERT INTO empleados (id_usuario, nombre, cargo, turno, salario, fecha_contratacion) VALUES 
(1, 'Juan Pérez', 'Director', 'Mañana', 2500.00, '2023-01-01'),
(2, 'María López', 'Recepcionista', 'Tarde', 1500.00, '2023-02-15'),
(3, 'Carlos Ruiz', 'Personal de limpieza', 'Mañana', 1200.00, '2023-03-10'),
(4, 'Ana Gómez', 'Técnico de mantenimiento', 'Mañana', 1400.00, '2023-04-05');

-- Habitaciones de prueba
INSERT INTO habitaciones (numero, tipo, precio, estado, capacidad, descripcion) VALUES 
('101', 'Individual', 60.00, 'Disponible', 1, 'Habitación individual con vistas al jardín'),
('102', 'Individual', 65.00, 'Disponible', 1, 'Habitación individual con vistas a la calle'),
('201', 'Doble', 90.00, 'Disponible', 2, 'Habitación doble con cama matrimonial'),
('202', 'Doble', 95.00, 'Mantenimiento', 2, 'Habitación doble con dos camas individuales'),
('301', 'Suite', 150.00, 'Disponible', 2, 'Suite con salón y jacuzzi'),
('401', 'Familiar', 180.00, 'Disponible', 4, 'Habitación familiar con dos habitaciones');

-- Reservas de prueba
INSERT INTO reservas (id_cliente, id_habitacion, fecha_entrada, fecha_salida, estado, monto, observaciones) VALUES 
(1, 1, '2025-03-15', '2025-03-18', 'Confirmada', 180.00, 'Cliente habitual'),
(2, 3, '2025-03-20', '2025-03-25', 'Pendiente', 450.00, 'Solicita cuna'),
(3, 5, '2025-04-01', '2025-04-05', 'Confirmada', 600.00, 'Celebración aniversario'),
(4, 6, '2025-04-10', '2025-04-15', 'Confirmada', 900.00, 'Familia con dos niños');

-- Facturación de prueba
INSERT INTO facturacion (id_reserva, id_cliente, monto, metodo_pago, fecha, numero_factura, estado) VALUES 
(1, 1, 180.00, 'Tarjeta de Crédito', '2025-03-18', 'FAC-2025-001', 'Pagada'),
(3, 3, 600.00, 'Efectivo', '2025-04-05', 'FAC-2025-002', 'Pagada'),
(4, 4, 900.00, 'Transferencia', '2025-04-01', 'FAC-2025-003', 'Pendiente');

-- Registros de actividad de prueba
INSERT INTO registros_actividad (id_usuario, operacion, detalles) VALUES 
(1, 'Inicio de sesión', 'Acceso al sistema'),
(2, 'Nueva reserva', 'Creación de reserva ID: 1'),
(1, 'Modificación de habitación', 'Cambio de estado de habitación 202 a Mantenimiento'),
(2, 'Emisión de factura', 'Factura FAC-2025-001 emitida');

-- Índices para mejorar el rendimiento
CREATE INDEX idx_reservas_cliente ON reservas(id_cliente);
CREATE INDEX idx_reservas_habitacion ON reservas(id_habitacion);
CREATE INDEX idx_facturacion_reserva ON facturacion(id_reserva);
CREATE INDEX idx_registros_usuario ON registros_actividad(id_usuario);

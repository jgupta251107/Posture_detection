import streamlit as st

def register_patient():
    st.subheader('User Registration')
    name = st.text_input('Name')
    age = st.number_input('Age', min_value=0, max_value=150, step=1)
    gender = st.selectbox('Gender', ['Male', 'Female', 'Other'])
    address = st.text_area('Address')
    contact = st.text_input('Contact Number')

    if st.button('Register'):
        # Here you can perform actions like saving patient data to a database
        st.success(f'Patient {name} registered successfully!')

def schedule_appointment():
    st.subheader('Schedule Appointment')
    USER_id = st.text_input('USER ID')
    date = st.date_input('Date')
    time = st.time_input('Time')
    doctor = st.text_input('Doctor')

    if st.button('Schedule'):
        # Here you can perform actions like saving appointment data to a database
        st.success('Appointment scheduled successfully!')

def view_patient_info():
    st.subheader('View USER Information')
    patient_id = st.text_input('USER_ID')

    if st.button('View'):
        # Here you can fetch patient information from a database and display it
        st.write(f'User ID: {User_id}')
        st.write('Name: John Doe')
        st.write('Age: 35')
        st.write('Gender: Male')
        st.write('Address: 123 Main St')
        st.write('Contact: 555-1234')



def main():
    st.title('Posture Management System')

    task = st.sidebar.selectbox(
        'Task',
        ['Register Patient', 'Schedule Appointment', 'View Patient Info']
    )

    if task == 'Register USER':
        register_user()
    elif task == 'Schedule Appointment':
        schedule_appointment()
    elif task == 'View Patient Info':
        view_user_info()

if _name_ == "_main_":
    main()

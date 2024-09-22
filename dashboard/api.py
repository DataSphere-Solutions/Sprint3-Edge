import dash
from dash import dcc, html
from dash.dependencies import Input, Output, State
import plotly.graph_objs as go
import requests
from datetime import datetime
import pytz

# Constants for IP and port
IP_ADDRESS = "20.197.230.60"
PORT_STH = 8666
DASH_HOST = "0.0.0.0"  # Set this to "0.0.0.0" to allow access from any IP


# Function to get speed data from the API
def get_speed_data(lastN):
    url = f"http://{IP_ADDRESS}:{PORT_STH}/STH/v1/contextEntities/type/Motor/id/urn:ngsi-ld:Motor:003/attributes/speed?lastN={lastN}"
    headers = {
        'fiware-service': 'smart',
        'fiware-servicepath': '/'
    }
    response = requests.get(url, headers=headers)
    if response.status_code == 200:
        data = response.json()
        try:
            values = data['contextResponses'][0]['contextElement']['attributes'][0]['values']
            return values
        except KeyError as e:
            print(f"Key error: {e}")
            return []
    else:
        print(f"Error accessing {url}: {response.status_code}")
        return []


# Function to get direction data from the API
def get_direction_data(lastN):
    url = f"http://{IP_ADDRESS}:{PORT_STH}/STH/v1/contextEntities/type/Motor/id/urn:ngsi-ld:Motor:003/attributes/direction?lastN={lastN}"
    headers = {
        'fiware-service': 'smart',
        'fiware-servicepath': '/'
    }
    response = requests.get(url, headers=headers)
    if response.status_code == 200:
        data = response.json()
        try:
            values = data['contextResponses'][0]['contextElement']['attributes'][0]['values']
            return values
        except KeyError as e:
            print(f"Key error: {e}")
            return []
    else:
        print(f"Error accessing {url}: {response.status_code}")
        return []




# Function to convert UTC timestamps to Brasilia time
def convert_to_brasilia_time(timestamps):
    utc = pytz.utc
    brasilia = pytz.timezone('America/Sao_Paulo')
    converted_timestamps = []
    for timestamp in timestamps:
        try:
            timestamp = timestamp.replace('T', ' ').replace('Z', '')
            converted_time = utc.localize(datetime.strptime(timestamp, '%Y-%m-%d %H:%M:%S.%f')).astimezone(brasilia)
        except ValueError:
            # Handle case where milliseconds are not present
            converted_time = utc.localize(datetime.strptime(timestamp, '%Y-%m-%d %H:%M:%S')).astimezone(brasilia)
        converted_timestamps.append(converted_time)
    return converted_timestamps


# Set lastN value
lastN = 10  # Get 10 most recent points at each interval

app = dash.Dash(__name__)

app.layout = html.Div([
    html.H1('Data Sphere - Monitoramento do Motor dos Carrinhos Elétricos', style={
        'textAlign': 'center',
        'color': 'navy',  # Cor do texto
        'fontSize': '40px',  # Tamanho da fonte
        'fontWeight': 'bold'  # Negrito
    }),
    dcc.Graph(id='speed-graph'),
    dcc.Graph(id='direction-graph'),
    # Store to hold historical data
    dcc.Store(id='speed-data-store', data={'timestamps': [], 'speed_values': [], 'direction_values': []}),
    dcc.Interval(
        id='interval-component',
        interval=10 * 1000,  # in milliseconds (10 seconds)
        n_intervals=0
    )
])




@app.callback(
    Output('speed-data-store', 'data'),
    Input('interval-component', 'n_intervals'),
    State('speed-data-store', 'data')
)
def update_data_store(n, stored_data):
    # Get speed data
    data_speed = get_speed_data(lastN)
    # Get direction data
    data_direction = get_direction_data(lastN)

    if data_speed and data_direction:
        # Extract values and timestamps for speed
        speed_values = [float(entry['attrValue']) for entry in data_speed]  # Ensure values are floats
        timestamps = [entry['recvTime'] for entry in data_speed]

        # Convert timestamps to Brasilia time
        timestamps = convert_to_brasilia_time(timestamps)

        # Extract direction values
        direction_values = [entry['attrValue'] for entry in data_direction]  # Direction values are strings

        # Append new data to stored data
        stored_data['timestamps'].extend(timestamps)
        stored_data['speed_values'].extend(speed_values)
        stored_data['direction_values'].extend(direction_values)

        return stored_data

    return stored_data


@app.callback(
    Output('speed-graph', 'figure'),
    Input('speed-data-store', 'data')
)
def update_graph(stored_data):
    if stored_data['timestamps'] and stored_data['speed_values']:
        # Calculate mean speed
        mean_speed = sum(stored_data['speed_values']) / len(stored_data['speed_values'])

        # Create traces for the speed plot
        trace_speed = go.Scatter(
            x=stored_data['timestamps'],
            y=stored_data['speed_values'],
            mode='lines+markers',
            name='Speed',
            line=dict(color='orange')
        )
        trace_mean = go.Scatter(
            x=[stored_data['timestamps'][0], stored_data['timestamps'][-1]],
            y=[mean_speed, mean_speed],
            mode='lines',
            name='Mean Speed',
            line=dict(color='blue', dash='dash')
        )

        # Create figure
        fig_speed = go.Figure(data=[trace_speed, trace_mean])

        # Update layout
        fig_speed.update_layout(
            title='Velocidade do Motor ao Longo do Tempo',
            xaxis_title='Timestamp',
            yaxis_title='Speed',
            hovermode='closest'
        )

        return fig_speed

    return {}


@app.callback(
    Output('direction-graph', 'figure'),
    Input('speed-data-store', 'data')
)
def update_direction_graph(stored_data):
    if stored_data['timestamps'] and stored_data['direction_values']:
        # Create traces for the direction plot
        trace_direction = go.Scatter(
            x=stored_data['timestamps'],
            y=stored_data['direction_values'],
            mode='lines+markers',
            name='Direction',
            line=dict(color='green')
        )

        # Create figure
        fig_direction = go.Figure(data=[trace_direction])

        # Update layout
        fig_direction.update_layout(
            title='Direcao do Motor',
            xaxis_title='Timestamp',
            yaxis_title='Direction',
            hovermode='closest'
        )

        return fig_direction

    return {}

if __name__ == '__main__':
    app.run_server(debug=True, host=DASH_HOST, port=8050)

# riley/venv/bin/python3

from data.store.model import Data


def data(this_session, location, use, delete=False):

    if delete: this_session.query(Data).delete()

    try:
        data_table = Data(**{
            'file_location': location,
            'use': use
        })

        this_session.add(data_table)
        this_session.commit()

    except Exception as e:
        print(e)
        print("For file at {location}".format(location=location))

    finally:
        this_session.close()

# riley/venv/bin/python3

from sqlalchemy import create_engine
from sqlalchemy_utils import database_exists, create_database
from sqlalchemy.orm import sessionmaker
from data.store.model import Base
from data.store.data import data


def fill(location, use):
    engine = create_engine("sqlite:///roulette_data.db")

    Base.metadata.create_all(engine)

    if not database_exists(engine.url):
        create_database(engine.url)

    Session = sessionmaker(bind=engine)
    session = Session()

    data(session, location, use)


# TODO: allow multiple data packets to be added in a single call of <function fill>

disobedience_0_0_SIIR = {

    "scenario": {
        "default_delay": "inertial",
        "default_cell_type": "zhong",
        "default_state": {
            "population": 1,
            "age_group_proportions": [0.216, 0.279, 0.268, 0.193, 0.044],
            "susceptible": [1, 1, 1, 1, 1],
            "fatalities": [0, 0, 0, 0, 0],
            "infected": [
                [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
                [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
                [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
                [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
                [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
            ],
            "recovered": [
                [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
                [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
                [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
                [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
                [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
            ],
            "disobedient": 0.0,
            "hospital_capacity": 0.2,
            "fatality_modifier": 1.5
        },
        "default_vicinity": {
            "correlation": 1,
            "infection_correction_factors": {"0.1": [0.8, 0.1], "0.2": [0.5, 0.1], "0.3": [0.2, 0.1]},
        },
        "default_config": {
            "zhong": {
                "precision": 100000000,
                "virulence_rates": [
                    [0.05, 0.05, 0.05, 0.05, 0.05, 0.05, 0.05, 0.05, 0.05, 0.05, 0.05, 0.05],
                    [0.05, 0.05, 0.05, 0.05, 0.05, 0.05, 0.05, 0.05, 0.05, 0.05, 0.05, 0.05],
                    [0.05, 0.05, 0.05, 0.05, 0.05, 0.05, 0.05, 0.05, 0.05, 0.05, 0.05, 0.05],
                    [0.05, 0.05, 0.05, 0.05, 0.05, 0.05, 0.05, 0.05, 0.05, 0.05, 0.05, 0.05],
                    [0.05, 0.05, 0.05, 0.05, 0.05, 0.05, 0.05, 0.05, 0.05, 0.05, 0.05, 0.05]
                ],
                "recovery_rates":[
                    [0.07, 0.07, 0.07, 0.07, 0.07, 0.07, 0.07, 0.07, 0.07, 0.07, 0.07, 0.07],
                    [0.07, 0.07, 0.07, 0.07, 0.07, 0.07, 0.07, 0.07, 0.07, 0.07, 0.07, 0.07],
                    [0.07, 0.07, 0.07, 0.07, 0.07, 0.07, 0.07, 0.07, 0.07, 0.07, 0.07, 0.07],
                    [0.07, 0.07, 0.07, 0.07, 0.07, 0.07, 0.07, 0.07, 0.07, 0.07, 0.07, 0.07],
                    [0.07, 0.07, 0.07, 0.07, 0.07, 0.07, 0.07, 0.07, 0.07, 0.07, 0.07, 0.07]
                ],
                "mobility_rates": [
                    [0.6, 0.6, 0.6, 0.6, 0.6, 0.6, 0.6, 0.6, 0.6, 0.6, 0.6, 0.6],
                    [0.6, 0.6, 0.6, 0.6, 0.6, 0.6, 0.6, 0.6, 0.6, 0.6, 0.6, 0.6],
                    [0.6, 0.6, 0.6, 0.6, 0.6, 0.6, 0.6, 0.6, 0.6, 0.6, 0.6, 0.6],
                    [0.6, 0.6, 0.6, 0.6, 0.6, 0.6, 0.6, 0.6, 0.6, 0.6, 0.6, 0.6],
                    [0.6, 0.6, 0.6, 0.6, 0.6, 0.6, 0.6, 0.6, 0.6, 0.6, 0.6, 0.6]
                ],
                "fatality_rates": [
                    [0.005, 0.005, 0.005, 0.005, 0.005, 0.005, 0.005, 0.005, 0.005, 0.005, 0.005, 0.005],
                    [0.005, 0.005, 0.005, 0.005, 0.005, 0.005, 0.005, 0.005, 0.005, 0.005, 0.005, 0.005],
                    [0.005, 0.005, 0.005, 0.005, 0.005, 0.005, 0.005, 0.005, 0.005, 0.005, 0.005, 0.005],
                    [0.005, 0.005, 0.005, 0.005, 0.005, 0.005, 0.005, 0.005, 0.005, 0.005, 0.005, 0.005],
                    [0.005, 0.005, 0.005, 0.005, 0.005, 0.005, 0.005, 0.005, 0.005, 0.005, 0.005, 0.005]
                ],
                "SIIRS_model": False
            }
        }
    },

    "cells": [
        {
            "cell_id": "Country1",
            "state": {
                "population": 1,
                "age_group_proportions": [0.216, 0.279, 0.268, 0.193, 0.044],
                "susceptible": [0.5, 0.6, 0.4, 0.3, 0.2],
                "fatalities": [0, 0, 0, 0, 0],
                "infected": [
                    [0.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
                    [0.4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
                    [0.6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
                    [0.7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
                    [0.8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
                ],
                "recovered": [
                    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
                    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
                    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
                    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
                    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
                ],
                "disobedient": 0.0,
                "hospital_capacity": 0.2,
                "fatality_modifier": 1.5
            },
            "neighborhood": [
                {"cell_id": "Country1"},
                {
                    "cell_id": "Country2",
                    "vicinity":
                        {
                            "correlation": 0.5,
                            "infection_correction_factors": {"0.1": [0.8, 0.1], "0.2": [0.5, 0.1], "0.3": [0.2, 0.1]},
                        }
                },
                {
                    "cell_id": "Country3",
                    "vicinity":
                        {
                            "correlation": 0.6,
                            "infection_correction_factors": {"0.1": [0.8, 0.1], "0.2": [0.5, 0.1], "0.3": [0.2, 0.1]},
                        }
                }
            ]
        },
        {
            "cell_id": "Country2",
            "neighborhood": [
                {
                    "cell_id": "Country1",
                    "vicinity":
                        {
                            "correlation": 0.5,
                            "infection_correction_factors": {"0.1": [0.8, 0.1], "0.2": [0.5, 0.1], "0.3": [0.2, 0.1]},
                        }
                },
                {"cell_id": "Country2"},
                {
                    "cell_id": "Country3",
                    "vicinity":
                        {
                            "correlation": 0.7,
                            "infection_correction_factors": {"0.1": [0.8, 0.1], "0.2": [0.5, 0.1], "0.3": [0.2, 0.1]},
                        }
                }
            ]
        },
        {
            "cell_id": "Country3",
            "neighborhood": [
                {
                    "cell_id": "Country1",
                    "vicinity":
                        {
                            "correlation": 0.7,
                            "infection_correction_factors": {"0.1": [0.8, 0.1], "0.2": [0.5, 0.1], "0.3": [0.2, 0.1]},
                        }
                },
                {
                    "cell_id": "Country2",
                    "vicinity":
                        {
                            "correlation": 0.6,
                            "infection_correction_factors": {"0.1": [0.8, 0.1], "0.2": [0.5, 0.1], "0.3": [0.2, 0.1]},
                        }
                },
                {"cell_id": "Country3"}
            ]
        }
    ]
}

disobedience_0_0_SIIRS = {

    "scenario": {
        "default_delay": "inertial",
        "default_cell_type": "zhong",
        "default_state": {
            "population": 1,
            "age_group_proportions": [0.216, 0.279, 0.268, 0.193, 0.044],
            "susceptible": [1, 1, 1, 1, 1],
            "fatalities": [0, 0, 0, 0, 0],
            "infected": [
                [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
                [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
                [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
                [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
                [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
            ],
            "recovered": [
                [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
                [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
                [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
                [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
                [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
            ],
            "disobedient": 0.0,
            "hospital_capacity": 0.2,
            "fatality_modifier": 1.5
        },
        "default_vicinity": {
            "correlation": 1,
            "infection_correction_factors": {"0.1": [0.8, 0.1], "0.2": [0.5, 0.1], "0.3": [0.2, 0.1]},
        },
        "default_config": {
            "zhong": {
                "precision": 100000000,
                "virulence_rates": [
                    [0.05, 0.05, 0.05, 0.05, 0.05, 0.05, 0.05, 0.05, 0.05, 0.05, 0.05, 0.05],
                    [0.05, 0.05, 0.05, 0.05, 0.05, 0.05, 0.05, 0.05, 0.05, 0.05, 0.05, 0.05],
                    [0.05, 0.05, 0.05, 0.05, 0.05, 0.05, 0.05, 0.05, 0.05, 0.05, 0.05, 0.05],
                    [0.05, 0.05, 0.05, 0.05, 0.05, 0.05, 0.05, 0.05, 0.05, 0.05, 0.05, 0.05],
                    [0.05, 0.05, 0.05, 0.05, 0.05, 0.05, 0.05, 0.05, 0.05, 0.05, 0.05, 0.05]
                ],
                "recovery_rates":[
                    [0.07, 0.07, 0.07, 0.07, 0.07, 0.07, 0.07, 0.07, 0.07, 0.07, 0.07, 0.07],
                    [0.07, 0.07, 0.07, 0.07, 0.07, 0.07, 0.07, 0.07, 0.07, 0.07, 0.07, 0.07],
                    [0.07, 0.07, 0.07, 0.07, 0.07, 0.07, 0.07, 0.07, 0.07, 0.07, 0.07, 0.07],
                    [0.07, 0.07, 0.07, 0.07, 0.07, 0.07, 0.07, 0.07, 0.07, 0.07, 0.07, 0.07],
                    [0.07, 0.07, 0.07, 0.07, 0.07, 0.07, 0.07, 0.07, 0.07, 0.07, 0.07, 0.07]
                ],
                "mobility_rates": [
                    [0.6, 0.6, 0.6, 0.6, 0.6, 0.6, 0.6, 0.6, 0.6, 0.6, 0.6, 0.6],
                    [0.6, 0.6, 0.6, 0.6, 0.6, 0.6, 0.6, 0.6, 0.6, 0.6, 0.6, 0.6],
                    [0.6, 0.6, 0.6, 0.6, 0.6, 0.6, 0.6, 0.6, 0.6, 0.6, 0.6, 0.6],
                    [0.6, 0.6, 0.6, 0.6, 0.6, 0.6, 0.6, 0.6, 0.6, 0.6, 0.6, 0.6],
                    [0.6, 0.6, 0.6, 0.6, 0.6, 0.6, 0.6, 0.6, 0.6, 0.6, 0.6, 0.6]
                ],
                "fatality_rates": [
                    [0.005, 0.005, 0.005, 0.005, 0.005, 0.005, 0.005, 0.005, 0.005, 0.005, 0.005, 0.005],
                    [0.005, 0.005, 0.005, 0.005, 0.005, 0.005, 0.005, 0.005, 0.005, 0.005, 0.005, 0.005],
                    [0.005, 0.005, 0.005, 0.005, 0.005, 0.005, 0.005, 0.005, 0.005, 0.005, 0.005, 0.005],
                    [0.005, 0.005, 0.005, 0.005, 0.005, 0.005, 0.005, 0.005, 0.005, 0.005, 0.005, 0.005],
                    [0.005, 0.005, 0.005, 0.005, 0.005, 0.005, 0.005, 0.005, 0.005, 0.005, 0.005, 0.005]
                ],
                "SIIRS_model": True
            }
        }
    },

    "cells": [
        {
            "cell_id": "Country1",
            "state": {
                "population": 1,
                "age_group_proportions": [0.216, 0.279, 0.268, 0.193, 0.044],
                "susceptible": [0.5, 0.6, 0.4, 0.3, 0.2],
                "fatalities": [0, 0, 0, 0, 0],
                "infected": [
                    [0.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
                    [0.4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
                    [0.6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
                    [0.7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
                    [0.8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
                ],
                "recovered": [
                    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
                    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
                    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
                    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
                    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
                ],
                "disobedient": 0.0,
                "hospital_capacity": 0.2,
                "fatality_modifier": 1.5
            },
            "neighborhood": [
                {"cell_id": "Country1"},
                {
                    "cell_id": "Country2",
                    "vicinity":
                        {
                            "correlation": 0.5,
                            "infection_correction_factors": {"0.1": [0.8, 0.1], "0.2": [0.5, 0.1], "0.3": [0.2, 0.1]},
                        }
                },
                {
                    "cell_id": "Country3",
                    "vicinity":
                        {
                            "correlation": 0.6,
                            "infection_correction_factors": {"0.1": [0.8, 0.1], "0.2": [0.5, 0.1], "0.3": [0.2, 0.1]},
                        }
                }
            ]
        },
        {
            "cell_id": "Country2",
            "neighborhood": [
                {
                    "cell_id": "Country1",
                    "vicinity":
                        {
                            "correlation": 0.5,
                            "infection_correction_factors": {"0.1": [0.8, 0.1], "0.2": [0.5, 0.1], "0.3": [0.2, 0.1]},
                        }
                },
                {"cell_id": "Country2"},
                {
                    "cell_id": "Country3",
                    "vicinity":
                        {
                            "correlation": 0.7,
                            "infection_correction_factors": {"0.1": [0.8, 0.1], "0.2": [0.5, 0.1], "0.3": [0.2, 0.1]},
                        }
                }
            ]
        },
        {
            "cell_id": "Country3",
            "neighborhood": [
                {
                    "cell_id": "Country1",
                    "vicinity":
                        {
                            "correlation": 0.7,
                            "infection_correction_factors": {"0.1": [0.8, 0.1], "0.2": [0.5, 0.1], "0.3": [0.2, 0.1]},
                        }
                },
                {
                    "cell_id": "Country2",
                    "vicinity":
                        {
                            "correlation": 0.6,
                            "infection_correction_factors": {"0.1": [0.8, 0.1], "0.2": [0.5, 0.1], "0.3": [0.2, 0.1]},
                        }
                },
                {"cell_id": "Country3"}
            ]
        }
    ]
}

scenario_list = [disobedience_0_0_SIIR, disobedience_0_0_SIIRS]
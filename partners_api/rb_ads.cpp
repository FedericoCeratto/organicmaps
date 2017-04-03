#include "partners_api/rb_ads.hpp"

namespace
{
auto const kFoodPlacementId = "1";
auto const kShopsPlacementId = "2";
auto const kCityTransportPlacementId = "13";
auto const kGlobalTransportPlacementId = "12";
auto const kHotelsPlacementId = "4";
auto const kSightsPlacementId = "5";
auto const kLargeToponymsPlacementId = "6";
auto const kHealthPlacementId = "7";
auto const kFinancialPlacementId = "8";
auto const kEntertainmentPlacementId = "9";
auto const kBuildingPlacementId = "11";
auto const kBannerIdForOtherTypes = "14";

std::vector<storage::TCountryId> const kSupportedCountries =
{
  "Azerbaijan",
  "Armenia",
  "Belarus",
  "Kazakhstan",
  "Kyrgyzstan",
  "Moldova",
  "Russia",
  "Tajikistan",
  "Turkmenistan",
  "Uzbekistan",
  "Ukraine"
};
}  // namespace

namespace ads
{
Rb::Rb()
{
  AppendEntry({{"amenity", "cafe"},
               {"amenity", "fast_food"},
               {"amenity", "restaurant"},
               {"amenity", "bar"},
               {"amenity", "pub"}},
              kFoodPlacementId);

  AppendEntry({{"shop"},
               {"amenity", "marketplace"}},
              kShopsPlacementId);

  AppendEntry({{"aerialway"},
               {"highway", "bus_stop"},
               {"highway", "speed_camera"},
               {"public_transport"}},
              kCityTransportPlacementId);

  AppendEntry({{"aeroway"},
               {"railway"},
               {"man_made", "pier"}},
              kGlobalTransportPlacementId);

  AppendEntry({{"tourism", "hotel"},
               {"tourism", "hostel"},
               {"tourism", "motel"},
               {"tourism", "apartment"},
               {"tourism", "resort"}},
              kHotelsPlacementId);

  AppendEntry({{"tourism", "chalet"},
               {"tourism", "zoo"},
               {"tourism", "artwork"},
               {"tourism", "information"},
               {"tourism", "attraction"},
               {"tourism", "viewpoint"},
               {"tourism", "museum"},
               {"amenity", "fountain"},
               {"amenity", "townhall"},
               {"historic"}},
              kSightsPlacementId);

  AppendEntry({{"place"}}, kLargeToponymsPlacementId);

  AppendEntry({{"amenity", "dentist"},
               {"amenity", "doctors"},
               {"amenity", "clinic"},
               {"amenity", "hospital"},
               {"amenity", "pharmacy"},
               {"amenity", "veterinary"}},
              kHealthPlacementId);

  AppendEntry({{"amenity", "bank"}, {"amenity", "atm"}}, kFinancialPlacementId);

  AppendEntry({{"amenity", "cinema"},
               {"amenity", "brothel"},
               {"amenity", "casino"},
               {"amenity", "nightclub"},
               {"amenity", "theatre"},
               {"boundary", "national_park"},
               {"leisure"}},
              kEntertainmentPlacementId);

  AppendEntry({{"building"}}, kBuildingPlacementId);

  AppendSupportedCountries(kSupportedCountries);
}

std::string Rb::GetBannerIdForOtherTypes() const
{
  return kBannerIdForOtherTypes;
}
}  // namespace ads
